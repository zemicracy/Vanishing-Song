#include "Player.h"
#include "Debug.h"
#include "Utility.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
#include <Windows.h>
using namespace aetherClass;

namespace{
	const int kWaitAnimationFrame = 60;
	const int kMoveAnimationFrame = 5;
	const Vector3 kColliderOffset = Vector3(0, -5, 0); 
	const Vector3 kPlayerInitialY = Vector3(0, 26, 0);
}

Player::Player()
{
	m_pGearFrame = nullptr;
	m_pTopGear = nullptr;
	m_pActionCommand = nullptr;
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

	// 初期位置の設定
	m_playerTransform._translation = kPlayerInitialY;

	// パーツの初期位置
	result = mLoadProperty(m_pGearFrame, "data\\PlayerDefault.aether");
	if (!result)
	{
		Debug::mErrorPrint("Editorからの位置反映に失敗", __FILE__, __LINE__);
		return false;
	}


	// コライダーの初期化
	mSetUpBodyCollider(m_pBodyCollider, m_pTopGear->_pGear->property._transform._translation, kColliderOffset);

	/*	基本的なアニメーションの登録	*/
	mRegisterAnimation(Player::eState::eMove, kMoveAnimationFrame,"data\\PlayerDefault.aether", "data\\PlayerMove.aether");
	mRegisterAnimation(Player::eState::eWait, kWaitAnimationFrame,"data\\PlayerDefault.aether", "data\\PlayerWait.aether");
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
	m_defaultAnimation.clear();

	m_pGearPartsHash.clear();

	if (m_pGearFrame)
	{
		m_pGearFrame->Release();
		m_pGearFrame.reset();
		m_pGearFrame = nullptr;
	}

	// 
	if (m_pTopGear)
	{
		m_pTopGear->Release();
		m_pTopGear.reset();
		m_pTopGear = nullptr;
	}

	// アクションコマンド
	if (m_pActionCommand)
	{
		m_pActionCommand.reset();
		m_pActionCommand = nullptr;
	}

	if (m_pBodyCollider)
	{
		m_pBodyCollider->Finalize();
		m_pBodyCollider.reset();
		m_pBodyCollider = nullptr;
	}

	// ステータスのリセット
	m_status.Reset();
	m_defaultAnimation.clear();

	m_prevCommand = eCommandType::eNull;
	m_prevState = eState::eNull;
	m_cameraRotation = kVector3Zero;
	return;
}


/*
プレイヤーの更新処理
*/
void Player::mUpdate(const float timeScale){
	
	// キーの処理を取得
	KeyValues getKeyValues = mReadKey(timeScale);

	// カメラの処理
	m_cameraRotation += getKeyValues._cameraRotation;
	mUpdateView(m_playerView, m_cameraRotation, m_pTopGear->_pGear->property._transform._translation);


	// 基本的なアニメーションの再生
	mDefaultAnimation(getKeyValues._state);

	// 移動に使う値のを取得
	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(m_cameraRotation*kAetherRadian);

	// カメラの回転行列を掛け合わせて、カメラの向きと進行方向を一致させる
	Vector3 translation = getKeyValues._transform._translation.TransformCoordNormal(rotationMatrix);
	m_playerTransform._translation += translation;
	m_playerTransform._rotation += getKeyValues._cameraRotation;

	// 移動処理
	m_charaEntity.mGearMove(m_pTopGear, m_playerTransform._translation);

	// 回転処理
	m_charaEntity.mGearRotation(m_pTopGear, m_pTopGear, m_playerTransform._rotation);

	// コライダーの更新処理
	mUpdateBodyCollider(m_pTopGear->_pGear->property._transform);
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
		output._transform._translation._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		output._transform._translation._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// 横の移動(X軸)
	if (GameController::GetKey().IsKeyDown('D')){
		output._transform._translation._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		output._transform._translation._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	// 移動があれば
	if (output._transform._translation == kVector3Zero){
		output._state = eState::eWait;
	}
	else{
		output._state = eState::eMove;
	}


	/*	カメラの回転	*/
	if (GameController::GetMouse().IsRightButtonDown()){
		Vector2 cameraRotation = GameController::GetMouse().GetMouseMovement();
		//LockMouseCursor(m_directX.GetWindowHandle(L"Scene"));
		cameraRotation /= kAetherPI;
		//output._cameraRotation._x += cameraRotation._y;
		output._cameraRotation._y += cameraRotation._x;

	}

	if (GameController::GetKey().IsKeyDown(VK_UP)){
		output._cameraRotation._x += (GameClock::GetDeltaTime()*timeScale);
	}
	else if (GameController::GetKey().IsKeyDown(VK_DOWN)){
		output._cameraRotation._x += -(GameClock::GetDeltaTime()*timeScale);
	}

	if (GameController::GetKey().IsKeyDown(VK_RIGHT)){
		output._cameraRotation._y += (GameClock::GetDeltaTime()*timeScale);
	}
	else if (GameController::GetKey().IsKeyDown(VK_LEFT)){
		output._cameraRotation._y += -(GameClock::GetDeltaTime()*timeScale);
	}

	return output;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_pTopGear)return;
	m_playerView.Render();
	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_pTopGear, modelShader, colliderShader);

	if (kCharaDebug)
	{
		m_pBodyCollider->Render(modelShader);
	}

	return;
}

//
eCommandType Player::mCommand(std::shared_ptr<ActionCommand> command, const float timeScale){

	// 今から行うアクションを取得
	m_status._nowCommand = command->mGetType();

	// 前回と違えば実行数を0にする
	if (m_status._nowCommand != m_prevCommand){
		m_actionCount._commandFrame = kZeroPoint;
		Debug::mPrint("Change Action");
	}

	// アクションの実行
	command->mAction(m_pGearFrame, timeScale,m_actionCount._commandFrame);

	if (GameController::GetKey().KeyDownTrigger('F'))
	{
		Debug::mPrint("Run Action :" + std::to_string(m_actionCount._commandFrame) + "回目");
	}
	
	m_actionCount._commandFrame += 1;

	// 状態を上書き
	m_prevCommand = m_status._nowCommand;

	return m_prevCommand;
}

/*
行動したアクションを登録
第一引数：アクションのタイプ
第二引数：何番目のアクションか
*/
void Player::mAddPrevActionCmmand(eCommandType action, const int id){
	m_status._prevActionList[id] = action;
	return;
}

/*
NULLで埋め尽くす
*/
void Player::mResetPrevActionList(){
	m_status._prevActionList.fill(eCommandType::eNull);

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
	gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Player\\body.fbx", Gear::eType::eBody, camera);

	// 腰のパーツ
	gearFrame->m_pWaist = m_charaEntity.mSetUpGear("Model\\Player\\waist.fbx", Gear::eType::eWaist, camera);

	// 腕のパーツ
	gearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eLeftUpperArm, camera);
	gearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eRightUpperArm, camera);
	gearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eLeftLowerArm, camera);
	gearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eRightLowerArm, camera);

	// 手のパーツ
	gearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eLeftHand, camera);
	gearFrame->m_pRightHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eRightHand, camera);

	// 足のパーツ
	gearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eLeftUpperLeg, camera);
	gearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eRightUpperLeg, camera);
	gearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eLeftLowerLeg, camera);
	gearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eRightLowerLeg, camera);

	// 最上位に当たるパーツの設定
	m_pTopGear = gearFrame->m_pBody;

	// それぞれのパーツとの親子関係構築
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pLeftUpperArm);

	// 右
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerArm, gearFrame->m_pRightHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperLeg, gearFrame->m_pRightLowerLeg);

	// 左
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerArm, gearFrame->m_pLeftHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperLeg, gearFrame->m_pLeftLowerLeg);

	//
	// 連想配列に登録
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eBody, m_pGearFrame->m_pBody);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eWaist, m_pGearFrame->m_pWaist);

	// 左
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftHand, m_pGearFrame->m_pLeftHand);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftLowerArm, m_pGearFrame->m_pLeftLowerArm);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftLowerLeg, m_pGearFrame->m_pLeftLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftUpperArm, m_pGearFrame->m_pLeftUpperArm);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftUpperLeg, m_pGearFrame->m_pLeftUpperLeg);

	// 右
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eRightHand, m_pGearFrame->m_pRightHand);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eRightLowerArm, m_pGearFrame->m_pRightLowerArm);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eRightLowerLeg, m_pGearFrame->m_pRightLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eRightUpperArm, m_pGearFrame->m_pRightUpperArm);
	m_charaEntity.mRegisterParts(m_pGearPartsHash, Gear::eType::eRightUpperLeg, m_pGearFrame->m_pRightUpperLeg);

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
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pWaist, index);
		}

		/*	左上半身*/
		if (index->_name == "LeftUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftUpperArm, index);
		}

		if (index->_name == "LeftLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftLowerArm, index);
		}

		if (index->_name == "LeftHand"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftHand, index);
		}

		/*	右上半身	*/
		if (index->_name == "RightUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightUpperArm, index);
		}

		if (index->_name == "RightLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightLowerArm, index);
		}

		if (index->_name == "RightHand"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightHand, index);
		}

		/*	右足	*/
		if (index->_name == "RightUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightUpperLeg, index);
		}

		if (index->_name == "RightLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightLowerLeg, index);
		}

		/*	左足	*/
		if (index->_name == "LeftUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftUpperLeg, index);
		}

		if (index->_name == "LeftLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftLowerLeg, index);
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
	
	// デバッグ用
	m_playerView.property._translation = Vector3(0, 100, -200);

	// カメラのオフセットの設定
	m_cameraOffset._translation = m_playerView.property._translation;
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
void Player::mDefaultAnimation(Player::eState& m_state){

	// 前回と違うときは更新
	if (m_prevState != m_state){
		m_actionCount._defaultFrame = NULL;
		m_prevState = m_state;
	}

	// 設定されていない場合何もしない
	if (m_defaultAnimation.find(m_state) == m_defaultAnimation.end()) return;

	Transform animationTransform;

	/*	アニメーション実行処理	*/
	const int allFrame = m_defaultAnimation[m_state]._animationFrame;

	for (auto index : m_defaultAnimation[m_state]._animation)
	{
		// 補間の値を取得
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end,allFrame , m_actionCount._defaultFrame);

		// アニメーションの適用
		if (m_pGearPartsHash.find(index._name) != m_pGearPartsHash.end()){
			m_pGearPartsHash[index._name]->_pGear->property._transform = animationTransform;
		}
	}

	
	if (m_state == eState::eWait){
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

	}
	else{
		m_actionCount._defaultFrame += 1;
	}

	return;
}

void Player::mUpdateView(ViewCamera& view,Vector3& rotation,Vector3 lookAtPosition){

	// カメラのリセット一応階といた
	if (GameController::GetKey().IsKeyDown(VK_RETURN)){
	rotation = kVector3Zero;
	}
	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(rotation*kAetherRadian);
	Vector3 position = m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	view.property._translation = lookAtPosition + position;
	view.property._rotation = rotation + m_cameraOffset._rotation;

	return;
}