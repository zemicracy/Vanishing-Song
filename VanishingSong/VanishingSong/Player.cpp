#include "Player.h"
#include "Debug.h"
#include "Utility.h"
#include "Bullet.h"
#include "Sord.h"
#include "Shield.h"
#include "Gun.h"
#include "ActionNull.h"

#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
#include <Windows.h>
using namespace aetherClass;

namespace{
	const int kWaitAnimationFrame = 60;
	const int kMoveAnimationFrame = 5;
	const float kCameraRotationMaxX = 15.0f;
	const float kCameraRotationMinX = -15.0f;
	const float kCameraY = 500.0f;
	const Vector3 kColliderOffset = Vector3(0, -5, 0); 
	const Vector3 kPlayerInitialY = Vector3(0, 20, 0);
	const Vector3 kBulletSpeed = Vector3(0, 0, 10);
	const float kDefaultMove = 100.0f;
	const float kDefaultMpHeal = 0.05f;
}

Player::Player()
{
	m_pGearFrame = nullptr;
	m_topGear = nullptr;
}


Player::~Player()
{
	mFinalize();
}

//
bool Player::mInitialize(){
	bool result;
	if (kCharaDebug)
	{
		Debug::mPrint("プレイヤー デバッグモードです");
		Debug::mPrint("プレイヤー 初期化を開始します");
	}

	mFinalize();

	
	// ギア系の初期化用
	result = mInitializeGearFrame(m_pGearFrame, &m_playerView);
	if (!result)
	{
		Debug::mErrorPrint("ギアの初期化に失敗", __FILE__, __LINE__);
		return false;
	}

	// パーツの初期位置
	result = mLoadProperty(m_pGearFrame, "data\\Player\\Stay.aether");

	if (!result)
	{
		Debug::mErrorPrint("Editorからの位置反映に失敗", __FILE__, __LINE__);
		return false;
	}

	// コライダーの初期化
	mSetUpBodyCollider(m_pBodyCollider, m_topGear->_pGear->property._transform._translation, kColliderOffset);

	/*	基本的なアニメーションの登録	*/
	mRegisterAnimation(Player::eState::eMove, kMoveAnimationFrame,"data\\Player\\Stay.aether", "data\\Player\\Move.aether");
	mRegisterAnimation(Player::eState::eWait, kWaitAnimationFrame,"data\\Player\\Stay.aether", "data\\Player\\Wait.aether");
	mRegisterAnimation(Player::eState::eHitDamage, kWaitAnimationFrame, "data\\Player\\Stay.aether", "data\\Player\\Wait.aether");
	

	// 武器系の初期化用
	mSetupWeapon<Sord>(m_wepons._sord, "Model\\Weapon\\bullet.fbx");
	mSetupWeapon<Shield>(m_wepons._shield, "Model\\Weapon\\bullet.fbx");
	mSetupWeapon<Gun>(m_wepons._gun, "Model\\Weapon\\bullet.fbx");

	mSetupBullet(mGetView());
	m_isHitWall = false;
	m_isCall = true;

	// とりあえず
	m_status._maxhp = 100;
	m_status._maxmp = 100;
	m_status._hp = 100;
	m_status._mp = 100;
	m_initalTransform = m_playerTransform;
	m_command = std::make_shared<ActionNull>();
	
	if (kCharaDebug)
	{
		Debug::mPrint("プレイヤー 初期化終了しました");
	}
	return true;
}

/*
解放処理
*/
void Player::mFinalize(){
	
	if (!m_defaultAnimation.empty()){
		for (auto index : m_defaultAnimation){
			index.second._animation.clear();
		}
		m_defaultAnimation.clear();
	}
	
	if (m_pGearFrame)
	{
		m_pGearFrame->Release();
		m_pGearFrame.reset();
		m_pGearFrame = nullptr;
	}

	if (m_pBodyCollider)
	{
		m_pBodyCollider->Finalize();
		m_pBodyCollider.reset();
		m_pBodyCollider = nullptr;
	}

	if (!m_pBullets.empty()){
		for (auto index : m_pBullets){
			if (!index._bullet)continue;
			index._bullet->mDestroy();
			index._bullet.reset();
			index._bullet = nullptr;

			index._isRun = false;
			index._number = NULL;
		}
	}

	// ステータスのリセット
	m_status.Reset();

	m_prevCommand = eCommandType::eNull;
	m_prevState = eState::eNull;
	m_cameraRotation = kVector3Zero;
	
	
	return;
}


/*
プレイヤーの更新処理
*/
void Player::mUpdate(const float timeScale, std::shared_ptr<ActionCommand> command){

	mCheckDead();

	// キーの処理を取得
	KeyValues getKeyValues = mReadKey(timeScale);

	// カメラの処理
	m_cameraRotation += getKeyValues._cameraRotation;

	// カメラの更新
	mUpdateView(m_playerView, m_cameraRotation, m_topGear->_pGear->property._transform._translation);

	// 移動があれば
	Player::eState state;

	// ダメージをくらっている状態じゃなかったら
	if (m_status._action != eActionType::eHitDamage){
		if (getKeyValues._transform._translation == kVector3Zero){
			state = eState::eWait;
		}
		else{
			state = eState::eMove;
		}
	}
	else{
		state = eState::eHitDamage;
	}

	// コマンドの処理
	mCommand(command, timeScale);
	
	// 基本的なアニメーションの再生
	mDefaultAnimation(state);

	// 移動に使う値のを取得
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0,m_cameraRotation._y,0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);
	
	// 壁に当たっているかの判定
	if (m_isHitWall || m_status._command != eCommandType::eNull){
		Vector3 revision = m_prevTransform._translation.Normalize();
		m_playerTransform._translation = m_prevTransform._translation - Vector3(revision._x, 0, revision._z);
		m_isHitWall = false;
	}
	else{
		// カメラの回転行列を掛け合わせて、カメラの向きと進行方向を一致させる
		Vector3 translation = getKeyValues._transform._translation.TransformCoordNormal(rotationMatrix);
		m_playerTransform._translation += translation;
	}

	// 向きだけ変える
	m_playerTransform._rotation._y += getKeyValues._cameraRotation._y;
	
	// 移動処理
	m_charaEntity.mGearMove(m_topGear, m_playerTransform._translation);
	
	// 回転処理
	m_charaEntity.mGearRotation(m_topGear, m_topGear, m_playerTransform._rotation);

	// コライダーの更新処理
	mUpdateBodyCollider(m_topGear->_pGear->property._transform);

	// 初めて呼ばれたならそれぞれの武器処理を開始
	if (!m_isCall){
		mWeaponFirstRun(m_status._command, m_actionCount._commandFrame);
	}

	// それぞれの状況に合わせた武器の更新処理
	mWeponUpdate(m_status._command, timeScale);

	// 弾の更新
	mUpdateBullet(timeScale, rotationMatrix, m_pBullets);

	mCheckDead();
	return;
}

/*
キーを読み込む
返りは今のところtransform
*/
Player::KeyValues Player::mReadKey(const float timeScale){

	KeyValues output;

	// 奥行の移動(Z軸)
	if (GameController::GetKey().IsKeyDown('W')){
		output._transform._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		output._transform._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	// 横の移動(X軸)
	if (GameController::GetKey().IsKeyDown('D')){
		output._transform._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		output._transform._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	/*	カメラの回転	*/
	Vector2 mousePosition = GameController::GetMouse().GetMousePosition();

	/*	コマンドやオーダーリストの箇所以外のみに反応する*/
	if (mousePosition._y < kCameraY){
		if (GameController::GetMouse().IsRightButtonDown()){
			gLockMouseCursor(m_directXEntity.GetWindowHandle(kWindowName), true);
			Vector2 cameraRotation = GameController::GetMouse().GetMouseMovement();
			cameraRotation /= (float)kAetherPI;
			output._cameraRotation._x += cameraRotation._y;
			output._cameraRotation._y += cameraRotation._x;

		}
	}

	return output;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_topGear)return;
	m_playerView.Render();
	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_topGear, modelShader, colliderShader);

	// 武器の描画
	mWeponRender(m_status._command, colliderShader);

	for (auto index : m_pBullets){
		if (!index._isRun)continue;
		index._bullet->mRender(modelShader);
	}

	if (kCharaDebug)
	{
		m_pBodyCollider->Render(colliderShader);
	}

	return;
}

//
void Player::mCommand(std::shared_ptr<ActionCommand> command, const float timeScale){
	if (!command)return;

	// 今から行うアクションを取得
	m_commandType = command->mGetType();

	// 回復
	if (m_status._command == eCommandType::eNull&&m_status._mp<m_status._maxmp){
		m_status._mp += kDefaultMpHeal;
	}


	// Null以外の時にコマンドを変える
	if (m_commandType != eCommandType::eNull)
	{
		m_command = command;
		m_status._command = m_commandType;
	}

	// 前回と違えば実行数を0にする
	if (m_status._command != m_prevCommand){
		
		m_actionCount._commandFrame = kZeroPoint;
		m_prevTransform = m_playerTransform;
		m_command->mCallCount(0);

		if (m_status._command != eCommandType::eNull){
			m_status._mp -= m_command->mGetExUseMP();
		}
	}

	m_isCall = m_command->mIsCall();

	// アクションの実行
	m_command->mAction(m_pGearHash, timeScale, m_actionCount._commandFrame);

	m_actionCount._commandFrame += 1;
	const int callCount = m_command->mCallCount();
	m_command->mCallCount(callCount + 1);

	// 状態を上書き
	m_prevCommand = m_status._command;

	if (m_command->mIsEnd()){

		m_status._command = eCommandType::eNull;
	}
	return;
}

/*
行動したアクションを登録
第一引数：アクションのタイプ
第二引数：何番目のアクションか
*/
void Player::mAddPrevActionCmmand(eCommandType action, const int id){
	m_status._prevCommandList[id] = action;
	return;
}

/*
NULLで埋め尽くす
*/
void Player::mResetPrevActionList(){
	m_status._prevCommandList.fill(eCommandType::eNull);
	return;
}

//
aetherClass::ViewCamera *Player::mGetView(){
	return &m_playerView;
}

std::shared_ptr<Cube> Player::mGetBodyColldier(){
	return m_pBodyCollider;
}

/*
ギア系の初期化をまとめたもの

*/
bool Player::mInitializeGearFrame(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){
	
	gearFrame = std::make_shared<GearFrame>();

	// 体のパーツ
	gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Player\\body.fbx", Gear::eType::eBody, camera,"Model\\Player\\tex");

	// 腰のパーツ
	gearFrame->m_pWaist = m_charaEntity.mSetUpGear("Model\\Player\\waist.fbx", Gear::eType::eWaist, camera, "Model\\Player\\tex");

	// 腕のパーツ
	gearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eLeftUpperArm, camera, "Model\\Player\\tex");
	gearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eRightUpperArm, camera, "Model\\Player\\tex");
	gearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eLeftLowerArm, camera, "Model\\Player\\tex");
	gearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eRightLowerArm, camera, "Model\\Player\\tex");

	// 手のパーツ
	gearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eLeftHand, camera, "Model\\Player\\tex");
	gearFrame->m_pRightHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eRightHand, camera, "Model\\Player\\tex");

	// 足のパーツ
	gearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eLeftUpperLeg, camera, "Model\\Player\\tex");
	gearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eRightUpperLeg, camera, "Model\\Player\\tex");
	gearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eLeftLowerLeg, camera, "Model\\Player\\tex");
	gearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eRightLowerLeg, camera, "Model\\Player\\tex");

	gearFrame->m_pLeftFoot= m_charaEntity.mSetUpGear("Model\\Player\\foot.fbx", Gear::eType::eLeftFoot, camera, "Model\\Player\\tex");
	gearFrame->m_pRightFoot= m_charaEntity.mSetUpGear("Model\\Player\\foot.fbx", Gear::eType::eRightFoot, camera, "Model\\Player\\tex");
	// 最上位に当たるパーツの設定
	m_topGear = gearFrame->m_pBody;

	// それぞれのパーツとの親子関係構築
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pLeftUpperArm);

	// 右
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerArm, gearFrame->m_pRightHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperLeg, gearFrame->m_pRightLowerLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerLeg, gearFrame->m_pRightFoot);

	// 左
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerArm, gearFrame->m_pLeftHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperLeg, gearFrame->m_pLeftLowerLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerLeg, gearFrame->m_pLeftFoot);

	//
	// 連想配列に登録
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eBody, m_pGearFrame->m_pBody);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eWaist, m_pGearFrame->m_pWaist);

	// 左
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftHand, m_pGearFrame->m_pLeftHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerArm, m_pGearFrame->m_pLeftLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerLeg, m_pGearFrame->m_pLeftLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperArm, m_pGearFrame->m_pLeftUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperLeg, m_pGearFrame->m_pLeftUpperLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftFoot, m_pGearFrame->m_pLeftFoot);
	// 右
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightHand, m_pGearFrame->m_pRightHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerArm, m_pGearFrame->m_pRightLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerLeg, m_pGearFrame->m_pRightLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperArm, m_pGearFrame->m_pRightUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperLeg, m_pGearFrame->m_pRightUpperLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightFoot, m_pGearFrame->m_pRightFoot);
	return true;
}

/*
	エディターから読み取り
*/
bool Player::mLoadProperty(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
	WorldReader read;
	bool result = read.Load(modelDataFile.c_str());
	if (!result)
	{
		Debug::mErrorPrint("ファイルの読み込みに失敗しました", __FILE__, __LINE__);
		return false;
	}

	for (auto index : read.GetInputWorldInfo()._object){

		/*	体	*/
		if (index->_name == "Body"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pWaist, index);
		}

		/*	左上半身*/
		if (index->_name == "LeftUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftUpperArm, index);
		}

		if (index->_name == "LeftLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftLowerArm, index);
		}

		if (index->_name == "LeftHand"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftHand, index);
		}

		/*	右上半身	*/
		if (index->_name == "RightUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightUpperArm, index);
		}

		if (index->_name == "RightLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightLowerArm, index);
		}

		if (index->_name == "RightHand"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightHand, index);
		}

		/*	右足	*/
		if (index->_name == "RightUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightUpperLeg, index);
		}

		if (index->_name == "RightLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightLowerLeg, index);
		}

		if (index->_name == "RightFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightFoot, index);
		}

		/*	左足	*/
		if (index->_name == "LeftUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftUpperLeg, index);
		}

		if (index->_name == "LeftLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftLowerLeg, index);
		}

		if (index->_name == "LeftFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftFoot, index);
		}

	}
	
	mInitialPlayerView(read.GetInputWorldInfo()._camera);
	read.UnLoad();

	return true;
}

/*
	カメラの初期化
*/
void Player::mInitialPlayerView(CameraValue input){
	// カメラの初期化
	m_playerView.property._translation = input._position;
	m_playerView.property._rotation = input._rotation;

	// 初期位置の設定
	m_playerTransform._translation = kPlayerInitialY;

	// カメラのオフセットの設定
	m_cameraOffset._translation = m_playerView.property._translation + m_playerTransform._translation+Vector3(0,0,-50);
	m_cameraOffset._rotation = m_playerView.property._rotation;

	return;
}

/*
	コライダーの初期化用
*/
void Player::mSetUpBodyCollider(std::shared_ptr<aetherClass::Cube>& collider, Vector3 original, Vector3 offset){
	collider = std::make_shared<Cube>();
	collider->Initialize();
	collider->property._transform._translation = original + offset;
	collider->property._transform._scale = 10;
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
	return;
}

/*
	コライダーの更新用
*/
void Player::mUpdateBodyCollider(Transform& transform){
	m_pBodyCollider->property._transform._translation = transform._translation + kColliderOffset;
	m_pBodyCollider->property._transform._rotation = transform._rotation;

	return;
}

/*
*/
void Player::mRegisterAnimation(Player::eState key, const int allFrame, std::string first, std::string last){
	AnimationFrame animation;
	bool result = false;

	// 既にある登録済みなら何もしない
	if (m_defaultAnimation.find(key) != m_defaultAnimation.end()) return;

	result = m_charaEntity.mLoadAnimation(animation._animation, first, last);
	if (!result)
	{
		Debug::mErrorPrint("読み込み失敗", __FILE__, __LINE__);
		return;
	}
	animation._animationFrame = allFrame;

	// 登録
	m_defaultAnimation.insert(std::make_pair(key, animation));

	return;
}

/*
	基本的なアニメーションの再生
*/
void Player::mDefaultAnimation(Player::eState& state){

	if (m_status._command != eCommandType::eNull)return;

	// 前回と違うときは更新
	if (m_prevState != state){
		m_actionCount._defaultFrame = NULL;
		m_prevState = state;
	}

	// 設定されていない場合何もしない
	if (m_defaultAnimation.find(state) == m_defaultAnimation.end()) return;

	Transform animationTransform;

	/*	アニメーション実行処理	*/
	const int allFrame = m_defaultAnimation[state]._animationFrame;

	for (auto index : m_defaultAnimation[state]._animation)
	{
		// 補間の値を取得
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end,allFrame , m_actionCount._defaultFrame);

		// アニメーションの適用
		if (m_pGearHash.find(index._name) != m_pGearHash.end()){
			m_pGearHash[index._name]->_pGear->property._transform = animationTransform;
		}
	}

	switch (state)
	{
		case eState::eWait:
			/*	カウンターの状態を切り替える	*/
			if (m_actionCount._defaultFrame > kWaitAnimationFrame){
				m_actionCount._changeDefaultFrame = true;
			}
			else if (m_actionCount._defaultFrame < kZeroPoint){
				m_actionCount._changeDefaultFrame = false;
			}

			// カウンターの状態に合わせてフレームカウントの更新
			if (m_actionCount._changeDefaultFrame){
				m_actionCount._defaultFrame -= 1;
			}
			else{
				m_actionCount._defaultFrame += 1;
			}
			break;

		case eState::eMove:
			m_actionCount._defaultFrame += 1;
			break;
		case eState::eHitDamage:
			m_actionCount._defaultFrame += 1;
			if (m_actionCount._defaultFrame > allFrame){
				m_status._action = eActionType::eNull;
			}
			break;
	default:
		break;
	}
	

	return;
}

void Player::mUpdateView(ViewCamera& view,Vector3& rotation,Vector3 lookAtPosition){

	// カメラのリセット一応階といた
	mCheckCameraRotation(rotation);
	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(rotation*kAetherRadian);
	Vector3 position = m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	view.property._translation = Vector3(lookAtPosition._x,NULL,lookAtPosition._z) + position;
	view.property._rotation = rotation + m_cameraOffset._rotation;

	return;
}

void Player::mUpdateBullet(const float timeScale, aetherClass::Matrix4x4& rotationMatrix, std::array<BulletPool, kMaxBullet>& bullets){
	// 弾の発射
	for (auto index : bullets){
		if (!index._isRun)continue;
		index._bullet->mGetTransform()._translation += index._moveValue;
		index._bullet->mUpdate(timeScale);
	}
	return;
}

void Player::mCheckCameraRotation(Vector3& rotation){
	// カメラ可動範囲の上限の確認
	if (rotation._x > kCameraRotationMaxX){
		rotation._x = kCameraRotationMaxX;
	}
	else if (rotation._x < kCameraRotationMinX){
		rotation._x = kCameraRotationMinX;
	}
	return;
}

template<class type>
void Player::mSetupWeapon(std::shared_ptr<Equipment>& weapon, std::string model){
	weapon = std::make_shared<type>();
	weapon->mCreate(mGetView(),model);
	return;
}

//
void Player::mSetupBullet(ViewCamera* view){
	for (auto&index : m_pBullets){
		index._bullet = std::make_shared<Bullet>();
		index._bullet->mCreate(view,"Model\\Weapon\\bullet.fbx");
		index._isRun = false;
		index._number = 0;
	}
}


void Player::mWeponUpdate(eCommandType type, const float timeScale){
	switch (type)
	{
	case eCommandType::eShortDistanceAttack:
		m_wepons._sord->mGetTransform()._translation = m_pGearFrame->m_pRightHand->_pGear->property._transform._translation;
		m_wepons._sord->mUpdate(timeScale);

		break;
	case eCommandType::eLongDistanceAttack:
		m_wepons._gun->mGetTransform()._translation = m_pGearFrame->m_pRightHand->_pGear->property._transform._translation;
		m_wepons._gun->mUpdate(timeScale);
		break;

	case eCommandType::eStrongShield:
	case eCommandType::eShield:
		m_wepons._shield->mGetTransform()._translation = m_pGearFrame->m_pLeftHand->_pGear->property._transform._translation;
		m_wepons._shield->mUpdate(timeScale);
		break;
	case eCommandType::eSkill:
		break;
	case eCommandType::eNull:
		break;
	default:
		break;
	}
}


void Player::mWeponRender(eCommandType type, aetherClass::ShaderBase* shader){
	switch (type)
	{
	case eCommandType::eShortDistanceAttack:
		m_wepons._sord->mRender(shader);
		break;
	case eCommandType::eLongDistanceAttack:
		m_wepons._gun->mRender(shader);
		
		break;
	case eCommandType::eStrongShield:
	case eCommandType::eShield:
		m_wepons._shield->mRender(shader);
		break;
	case eCommandType::eSkill:
		break;
	case eCommandType::eNull:
		break;
	default:
		break;
	}
}

//
void Player::mWeaponFirstRun(eCommandType type, const int callFrame){
	switch (type)
	{
	case eCommandType::eShortDistanceAttack:
		m_wepons._sord->mGetTransform()._translation = m_pGearFrame->m_pRightHand->_pGear->property._transform._translation;
		break;
	case eCommandType::eLongDistanceAttack:{
		m_wepons._gun->mGetTransform()._translation = m_pGearFrame->m_pRightHand->_pGear->property._transform._translation;
		for (auto& index : m_pBullets){
			if (index._isRun)continue;
			Matrix4x4 rotationMatrix;
			Vector3 rotationY = Vector3(0, m_cameraRotation._y,0);
			rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);
			Vector3 value = kBulletSpeed;
			const Vector3 vector = value.TransformCoordNormal(rotationMatrix);
			index._bullet->mGetTransform()._translation = m_wepons._gun->mGetTransform()._translation;
			index._moveValue = vector;
			index._isRun = true;
			break;
		}
	}
		break;
	case eCommandType::eShield:
	case eCommandType::eStrongShield:
		m_wepons._shield->mGetTransform()._translation = m_pGearFrame->m_pLeftHand->_pGear->property._transform._translation;	
		break;

	case eCommandType::eRightStep:
		break;
	case eCommandType::eLeftStep:
		break;
	case eCommandType::eSkill:
		break;
	case eCommandType::eNull:
		break;
	default:
		break;
	}
}

std::array<Player::BulletPool, kMaxBullet>& Player::mGetBullet(){
	return m_pBullets;
}

// 壁に当たった時の処理
void Player::mOnHitWall(){

	m_prevTransform = m_playerTransform;
	m_isHitWall = true;
	return;
}

eCommandType Player::mGetCommandType(){
	return m_status._command;
}

ResultData& Player::mGetResultData(){
	return m_resultData;
}

/*
	日の終了ごとに呼ぶ
*/
void Player::mDayReset(){

	// 弾の初期化
	for (auto&bullet : m_pBullets){
		bullet._isRun = false;
	}
	// リザルトに使うデータの初期化
	m_resultData.mReset();

	// 位置を初期位置にする
	m_charaEntity.mGearMove(m_topGear, Vector3(-m_playerTransform._translation._x, 0.0f, -m_playerTransform._translation._z));
	m_playerTransform= m_initalTransform;
	m_cameraRotation = kVector3Zero;
	mUpdateView(m_playerView, m_cameraRotation, m_playerTransform._translation);
	mUpdate(NULL, nullptr);

	return;
}

CharaStatus& Player::mGetStatus(){
	return m_status;
}

void Player::mOnHitEnemyAttack(const CharaStatus){
	if (m_status._action == eActionType::eHitDamage)return; // 連続で当たるのを防止
	m_status._hp -= 10;
	m_status._action = eActionType::eHitDamage;
	return;
}

void Player::mCheckDead(){
	if (m_status._hp <= 0){
		m_isDead = true;
	}
	else{
		m_isDead = false;
	}
	return;
}

/*
	プレイヤー死亡：true
*/
bool Player::mIsDead(){
	return m_isDead;
}