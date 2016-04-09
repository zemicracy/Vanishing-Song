#include "Player.h"
#include "Debug.h"
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
	mLoadModelProperty(m_pGearFrame, "data\\Player2.aether");

	if (kCharaDebug)
	{
		Debug::mPrint("プレイヤー 初期化終了しました");
		Debug::mPrint("");
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

	// 実際の移動処理
	m_charaEntity.mGearMove(m_pTopGear, transform._translation);

	// 回転処理
	// 体を含めたすべての回転
	//m_charaEntity.mBodyGearRotation(m_pTopGear, transform._rotation);

	// パーツだけの回転
	m_charaEntity.mPartsGearRotation(m_pTopGear, transform._rotation);
	return;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_pTopGear)return;

	m_playerView.Render();

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_pTopGear, modelShader, colliderShader);

	return;
}

//
eCommandType Player::m_Command(std::shared_ptr<ActionCommand> command, const float timeScale){

	// 今から行うアクションを取得
	m_status._nowCommand = command->mGetType();

	if (m_status._nowCommand != m_prevCommand){
		// 前回と違えば実行数を0にする
		m_actionCount = kZeroPoint;
		Debug::mPrint("Change Action");
	}

	// アクションの実行
	command->mAction(m_pGearFrame, timeScale, m_actionCount);


	Debug::mPrint("Run Action :" + std::to_string(m_actionCount) + "回目");
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

//
std::shared_ptr<aetherClass::ModelBase> Player::mGetCollider(const int id){
	return m_playerCollideList[id];
}

int Player::mGetColliderListSize()const{
	return m_playerCollideList.size();
}

/*
ギア系の初期化をまとめたもの

*/
bool Player::mInitializeGear(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){
	gearFrame = std::make_shared<GearFrame>();

	// 体のパーツ
	gearFrame->m_pBody = m_charaEntity.mSetUpGear("null", Gear::eType::eBody, camera);

	// 腰のパーツ
	gearFrame->m_pWaist = m_charaEntity.mSetUpGear("null", Gear::eType::eWaist, camera);

	// 腕のパーツ
	gearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperArm, camera);
	gearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperArm, camera);
	gearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerArm, camera);
	gearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerArm, camera);

	// 手のパーツ
	gearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftHand, camera);
	gearFrame->m_pRightHand = m_charaEntity.mSetUpGear("null", Gear::eType::eRightHand, camera);

	// 足のパーツ
	gearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperLeg, camera);
	gearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperLeg, camera);
	gearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerLeg, camera);
	gearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerLeg, camera);

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

		if (index->_name == "Body"){

			SetLoadModelValue(gearFrame->m_pBody, index);
		}

		if (index->_name == "LeftUpperArm"){
			SetLoadModelValue(gearFrame->m_pLeftUpperArm, index);
		}

		if (index->_name == "LeftLowerArm"){
			SetLoadModelValue(gearFrame->m_pLeftLowerArm, index);
		}
	}

	// カメラの初期化
	m_playerView.property._translation = read.GetInputWorldInfo()._camera._position;
	m_playerView.property._rotation = read.GetInputWorldInfo()._camera._rotation;
	
	read.UnLoad();

	return true;
}

void Player::SetLoadModelValue(std::shared_ptr<Gear>& gear, ObjectInfo* info){
	gear->_pColider->property._transform = info->_transform;

	if (gear->_pParent)
	{
		std::shared_ptr<Gear> pParent = gear->_pParent;
		gear->_difference._translation = gear->_pColider->property._transform._translation - pParent->_pColider->property._transform._translation;
		gear->_difference._rotation = gear->_pColider->property._transform._rotation - pParent->_pColider->property._transform._rotation;
	}

	// コライダーの登録
	m_playerCollideList.push_back(gear->_pColider);
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
	if (GameController::GetKey().IsKeyDown('Q')){
		transform._rotation._y = GameClock::GetDeltaTime()*timeScale * 100;
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		transform._rotation._y = -(GameClock::GetDeltaTime()*timeScale * 100);
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

	return transform;
}



