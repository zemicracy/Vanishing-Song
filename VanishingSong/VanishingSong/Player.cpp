#include "Player.h"
#include "Debug.h"
#include "Utility.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
using namespace aetherClass;

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
	if (kCharaDebug)
	{
		Debug::mPrint("プレイヤー デバッグモードです");
		Debug::mPrint("プレイヤー 初期化を開始します");
		Debug::mPrint("");
	}

	mFinalize();

	m_playerView.property._translation = Vector3(0, 0, -20);
	// ギア系の初期化用
	mInitializeGear(m_pGearFrame, &m_playerView);

	// パーツの初期位置
	mLoadModelProperty(m_pGearFrame, "data\\PlayerModel.aether");

	if (kCharaDebug)
	{
		Debug::mPrint("プレイヤー 初期化終了しました");
		Debug::mPrint("");
	}

	m_cameraOffset._translation = m_playerView.property._translation;
	m_cameraOffset._rotation = m_playerView.property._rotation;

	m_cameraRotation = kVector3Zero;

	// 初期位置の設定
	m_charaEntity.mGearMove(m_pTopGear, Vector3(0, 20, 0));

	mInitializeCollider(m_pCubeCollider);
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

	m_prevCommand = eCommandType::eNull;
	m_state = Player::eState::eNull;
	m_actionCount = NULL;
	return;
}


/*
プレイヤーの更新処理
*/
void Player::mUpdate(const float timeScale){

	// 移動に使う値のを取得
	Transform transform = mReadKey(timeScale);

	// 移動に変化量があれば
	if (transform._translation == kVector3Zero){
		Debug::mPrint("WAIT");
		
		m_state = eState::eWait;
	}
	else{
		Debug::mPrint("MOVE");
		m_state = eState::eMove;
	}

	// 実際の移動処理
	m_charaEntity.mGearMove(m_pTopGear, transform._translation);
	
	// コライダーも動かす
	m_pCubeCollider->property._transform._translation += transform._translation;
	
	if (GameController::GetKey().IsKeyDown('Q'))
	{
		m_cameraRotation._y += 1.03f;
		
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		m_cameraRotation._y -= 1.03f;
		
	}
	
	auto gearTranslation = m_pTopGear->_pGear->property._transform._translation;

	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(m_cameraRotation*kAetherRadian);
	Vector3 position = transform._translation + m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	m_playerView.property._translation = gearTranslation+position;
	m_playerView.property._rotation = m_cameraRotation + m_cameraOffset._rotation;

	return;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_pTopGear)return;

	if (kCharaDebug)
	{
		m_pCubeCollider->Render(colliderShader);
	}

	m_playerView.Render();

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
		m_actionCount = kZeroPoint;
		Debug::mPrint("Change Action");
	}

	// アクションの実行
	command->mAction(m_pGearFrame, timeScale,m_actionCount);

	if (GameController::GetKey().KeyDownTrigger('F'))
	{
		Debug::mPrint("Run Action :" + std::to_string(m_actionCount) + "回目");
	}
	
	m_actionCount += 1;

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
Transform Player::mReadKey(const float timeScale){

	Transform transform;

	// 奥行の移動(Z軸)
	if (GameController::GetKey().IsKeyDown('W')){
		transform._translation._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		transform._translation._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// 横の移動(X軸)
	if (GameController::GetKey().IsKeyDown('D')){
		transform._translation._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		transform._translation._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	// キャラがデバッグモードじゃないならここで終了
	if (!kCharaDebug) return transform;

	// 回転用(Y軸)
	/*if (GameController::GetKey().IsKeyDown('Q')){
		transform._rotation._y = GameClock::GetDeltaTime()*timeScale * 100;
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		transform._rotation._y = -(GameClock::GetDeltaTime()*timeScale * 100);
	}*/

	// デバッグ用
	Vector3 debug = m_pTopGear->_pGear->property._transform._translation;
	if (GameController::GetKey().KeyDownTrigger('P')){
		Debug::mPrint("---Playerの現在の座標---");
		Debug::mPrint("X :" + std::to_string(debug._x));
		Debug::mPrint("Y :" + std::to_string(debug._y));
		Debug::mPrint("Z :" + std::to_string(debug._z));
		Debug::mPrint("------------------------");
	}

	return transform;
}

void Player::mInitializeCollider(std::shared_ptr<aetherClass::Cube>& collider){
	collider = std::make_shared<Cube>();
	collider->Initialize();
	collider->property._transform._translation = m_pTopGear->_pGear->property._transform._translation + Vector3(1, -5, -0);
	collider->property._transform._scale = 10;
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
}


