#include "Player.h"
#include "Debug.h"
#include "Utility.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
using namespace aetherClass;

namespace{
	const int kWaitAnimationFrame = 60;
	const int kMoveAnimationFrame = 5;
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

	m_playerView.property._translation = Vector3(0, 0, -20);
	// ギア系の初期化用
	result = mInitializeGear(m_pGearFrame, &m_playerView);
	if (!result)
	{
		Debug::mErrorPrint("ギアの初期化に失敗", __FILE__, __LINE__);
		return false;
	}
	// パーツの初期位置
	result = mLoadModelProperty(m_pGearFrame, "data\\PlayerDefault.aether");
	if (!result)
	{
		Debug::mErrorPrint("ギアの位置反映に失敗", __FILE__, __LINE__);
		return false;
	}

	m_cameraOffset._translation = m_playerView.property._translation;
	m_cameraOffset._rotation = m_playerView.property._rotation;

	m_cameraRotation = kVector3Zero;

	// 初期位置の設定
	m_moveTransform._translation= Vector3(0,20,0);

	// 連想配列に登録
	mRegisterParts(m_pGearPartsHash, Gear::eType::eBody, m_pGearFrame->m_pBody);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eWaist, m_pGearFrame->m_pWaist);

	// 左
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftHand, m_pGearFrame->m_pLeftHand);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftLowerArm, m_pGearFrame->m_pLeftLowerArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftLowerLeg, m_pGearFrame->m_pLeftLowerLeg);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftUpperArm, m_pGearFrame->m_pLeftUpperArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftUpperLeg, m_pGearFrame->m_pLeftUpperLeg);

	// 右
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightHand, m_pGearFrame->m_pRightHand);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightLowerArm, m_pGearFrame->m_pRightLowerArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightLowerLeg, m_pGearFrame->m_pRightLowerLeg);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightUpperArm, m_pGearFrame->m_pRightUpperArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightUpperLeg, m_pGearFrame->m_pRightUpperLeg);

	// コライダーの初期化
	mSetUpCollider(m_pCubeCollider, m_pTopGear->_pGear->property._transform._translation, Vector3(1, -5, -0));

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

	if (m_pCubeCollider)
	{
		m_pCubeCollider->Finalize();
		m_pCubeCollider.reset();
		m_pCubeCollider = nullptr;
	}

	// ステータスのリセット
	m_status.Reset();
	m_defaultAnimation.clear();

	m_prevCommand = eCommandType::eNull;
	m_prevState = eState::eNull;

	return;
}


/*
プレイヤーの更新処理
*/
void Player::mUpdate(const float timeScale){
	
	Transform animTransform;

	// 移動に使う値のを取得
	m_moveTransform._translation += mReadKey(timeScale)._transform._translation;
	m_moveTransform._rotation += mReadKey(timeScale)._transform._rotation;
	
	eState state = mReadKey(timeScale)._state;
	// 基本的なアニメーションの再生
	mGetAnimationTransform(state, animTransform);


	// 実際の移動処理
	animTransform._translation += m_moveTransform._translation;
	animTransform._rotation+= m_moveTransform._rotation;

	m_charaEntity.mGearMove(m_pTopGear, animTransform._translation);
	m_charaEntity.mTopGearRotation(m_pTopGear, m_moveTransform._rotation);

	// コライダーは常にBodyと同じにしておく
	m_pCubeCollider->property._transform._translation = m_pGearFrame->m_pBody->_pGear->property._transform._translation;
	

	/*if (GameController::GetKey().IsKeyDown('Q'))
	{
		m_cameraRotation._y += 1.03f;
		
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		m_cameraRotation._y -= 1.03f;
		
	}*/
	mLookAtView(m_playerView,m_cameraRotation,m_pTopGear->_pGear->property._transform._translation);
	
	return;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_pTopGear)return;
	m_playerView.Render();

	if (kCharaDebug)
	{
	//	m_pCubeCollider->Render(modelShader);
	}

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_pTopGear, modelShader, colliderShader);

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

std::shared_ptr<Cube> Player::mGetColldier(){
	return m_pCubeCollider;
}

/*
ギア系の初期化をまとめたもの

*/
bool Player::mInitializeGear(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){
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

	// 体にパーツとの親子関係
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pLeftUpperArm);

	// 右腕の親子関係
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerArm, gearFrame->m_pRightHand);

	// 左腕の親子関係
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerArm, gearFrame->m_pLeftHand);

	// 右足の親子関係
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperLeg, gearFrame->m_pRightLowerLeg);

	// 左足の親子関係
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperLeg, gearFrame->m_pLeftLowerLeg);

	return true;
}

/*

*/
bool Player::mLoadModelProperty(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
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
			SetLoadModelValue(gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			SetLoadModelValue(gearFrame->m_pWaist, index);
		}

		/*	左上半身*/
		if (index->_name == "LeftUpperArm"){
			SetLoadModelValue(gearFrame->m_pLeftUpperArm, index);
		}

		if (index->_name == "LeftLowerArm"){
			SetLoadModelValue(gearFrame->m_pLeftLowerArm, index);
		}

		if (index->_name == "LeftHand"){
			SetLoadModelValue(gearFrame->m_pLeftHand, index);
		}

		/*	右上半身	*/
		if (index->_name == "RightUpperArm"){
			SetLoadModelValue(gearFrame->m_pRightUpperArm, index);
		}

		if (index->_name == "RightLowerArm"){
			SetLoadModelValue(gearFrame->m_pRightLowerArm, index);
		}

		if (index->_name == "RightHand"){
			SetLoadModelValue(gearFrame->m_pRightHand, index);
		}

		/*	右足	*/
		if (index->_name == "RightUpperLeg"){
			SetLoadModelValue(gearFrame->m_pRightUpperLeg, index);
		}

		if (index->_name == "RightLowerLeg"){
			SetLoadModelValue(gearFrame->m_pRightLowerLeg, index);
		}

		/*	左足	*/
		if (index->_name == "LeftUpperLeg"){
			SetLoadModelValue(gearFrame->m_pLeftUpperLeg, index);
		}

		if (index->_name == "LeftLowerLeg"){
			SetLoadModelValue(gearFrame->m_pLeftLowerLeg, index);
		}

	}

	// カメラの初期化
	m_playerView.property._translation = read.GetInputWorldInfo()._camera._position;
	m_playerView.property._rotation = read.GetInputWorldInfo()._camera._rotation;
	
	read.UnLoad();

	return true;
}

void Player::SetLoadModelValue(std::shared_ptr<Gear>& gear, ObjectInfo* info){

	gear->_pGear->property._transform = info->_transform;

	if (gear->_pParent)
	{
		std::shared_ptr<Gear> pParent = gear->_pParent;
		// 最上位との差
		gear->_topDifference._translation = gear->_pGear->property._transform._translation - m_pTopGear->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - m_pTopGear->_pGear->property._transform._rotation;

		// 親との差
		gear->_parentDifference._translation = gear->_pGear->property._transform._translation - m_pTopGear->_pGear->property._transform._translation;
		gear->_parentDifference._rotation = gear->_pGear->property._transform._rotation - m_pTopGear->_pGear->property._transform._rotation;
	}

	return;
}

/*
キーを読み込む
返りは今のところtransform
*/
Player::TransformState Player::mReadKey(const float timeScale){

	TransformState state;

	// 奥行の移動(Z軸)
	if (GameController::GetKey().IsKeyDown('W')){
		state._transform._translation._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		state._transform._translation._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// 横の移動(X軸)
	if (GameController::GetKey().IsKeyDown('D')){
		state._transform._translation._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		state._transform._translation._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	// 移動があれば
	if (state._transform._translation == kVector3Zero){
		state._state = eState::eWait;
	}
	else{
		state._state = eState::eMove;
	}

	// キャラがデバッグモードじゃないならここで終了
	if (!kCharaDebug) return state;

	// 回転用(Y軸)
	if (GameController::GetKey().IsKeyDown('Q')){
		state._transform._rotation._y = GameClock::GetDeltaTime()*timeScale * 1;
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		state._transform._rotation._y = -(GameClock::GetDeltaTime()*timeScale * 1);
	}

	// デバッグ用
	Vector3 debug = m_pTopGear->_pGear->property._transform._translation;
	if (GameController::GetKey().KeyDownTrigger('P')){
		Debug::mPrint("---Playerの現在の座標---");
		Debug::mPrint("X :" + std::to_string(debug._x));
		Debug::mPrint("Y :" + std::to_string(debug._y));
		Debug::mPrint("Z :" + std::to_string(debug._z));
		Debug::mPrint("------------------------");
	}

	return state;
}

/*
*/
void Player::mSetUpCollider(std::shared_ptr<aetherClass::Cube>& collider, Vector3 original, Vector3 offset){
	collider = std::make_shared<Cube>();
	collider->Initialize();
	collider->property._transform._translation = original + offset;
	collider->property._transform._scale = 10;
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
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
*/
void Player::mGetAnimationTransform(Player::eState m_state, Transform transform){

	// 前回と違うときは更新
	if (m_prevState != m_state){
		m_actionCount._defaultFrame = NULL;
		m_prevState = m_state;
	}

	Transform animationTransform;

	if (m_defaultAnimation.find(m_state) == m_defaultAnimation.end()) return;

	/*	アニメーション実行処理	*/
	const int allFrame = m_defaultAnimation[m_state]._animationFrame;
	for (auto index : m_defaultAnimation[m_state]._animation)
	{
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end,allFrame , m_actionCount._defaultFrame);

		animationTransform._translation += transform._translation;
		animationTransform._rotation += transform._rotation;
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


void Player::mRegisterParts(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash,Gear::eType type, std::shared_ptr<Gear>& parts){

	// 登録済みなら何もしない
	if (hash.find(type) != hash.end() || !parts)return;

	hash.insert(std::make_pair(type, parts));
	return;
}


void Player::mLookAtView(ViewCamera& view,Vector3 rotation,Vector3 lookAtPosition){
	
	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(rotation*kAetherRadian);
	Vector3 position = m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	view.property._translation = lookAtPosition + position;
	view.property._rotation = rotation + m_cameraOffset._rotation;

}