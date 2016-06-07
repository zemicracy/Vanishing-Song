#include "FieldPlayer.h"
#include "Debug.h"
#include "Utility.h"
#include "Const.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
#include <Windows.h>
#include <Singleton.h>
#include "ResourceManager.h"
using namespace aetherClass;

namespace{
	const int kWaitAnimationFrame = 60;
	const int kMoveAnimationFrame = 5;
	const float kCameraRotationMaxX = 15.0f;
	const float kCameraRotationMinX = -15.0f;
	const Vector3 kColliderOffset = Vector3(0, -5, 0); 
	const float kDefaultMove = 100.0f;

}

FieldPlayer::~FieldPlayer()
{
	mFinalize();
}

//
bool FieldPlayer::mInitialize(std::shared_ptr<GearFrame> gear, Vector3 position){
	bool result;
	mFinalize();

	// 初期位置の設定
	m_playerTransform._translation = position;
	
	auto gearFrame = gear;

	m_topGear = gearFrame->m_pBody;
	// ギア系の初期化用
	result = mInitializeGearFrame(gearFrame, &m_playerView);
	if (!result){
		return false;
	}

	// パーツの初期位置
	result = mLoadProperty(gearFrame, "data\\Player\\stay.aether");

	if (!result){
		return false;
	}
	
	// コライダーの初期化
	mSetUpBodyCollider(m_pBodyCollider, m_topGear->_pGear->property._transform._translation, kColliderOffset);
	
	/*	基本的なアニメーションの登録	*/
	//mRegisterAnimation(FieldPlayer::eState::eMove, kMoveAnimationFrame, "data\\Player\\stay.aether", "data\\Player\\Move.aether");
	mRegisterAnimation(FieldPlayer::eState::eWait, kWaitAnimationFrame, "data\\Player\\stay.aether", "data\\Player\\wait.aether");

	m_isHitWall = false;
	m_charaEntity.SetCamera(m_topGear, &m_playerView);

	m_sign = NULL;
	return true;
}

/*
解放処理
*/
void FieldPlayer::mFinalize(){
	
	if (!m_defaultAnimation.empty()){
		for (auto index : m_defaultAnimation){
			index.second._animation.clear();
		}
		m_defaultAnimation.clear();
	}


	if (m_pBodyCollider)
	{
		m_pBodyCollider->Finalize();
		m_pBodyCollider.reset();
		m_pBodyCollider = nullptr;
	}	
	m_prevState = eState::eNull;
	m_cameraRotation = kVector3Zero;

	m_fieldNumber = NULL;
	return;
}


/*
プレイヤーの更新処理
*/
void FieldPlayer::mUpdate(const float timeScale,const bool isWait){

	// キーの処理を取得
	std::pair<Transform, Vector3> getKeyValues;
	if (!isWait){
		getKeyValues = mReadKey(timeScale,m_sign);
	}

	// カメラの処理
	m_cameraRotation += getKeyValues.second;
	mUpdateView(m_playerView, m_cameraRotation, m_topGear->_pGear->property._transform._translation);

	
	// 移動があれば
	FieldPlayer::eState state;
	if (getKeyValues.first._translation == kVector3Zero){
		state = eState::eWait;
	}
	else{
		// 向きだけ変える
		m_playerTransform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eMove;
	}
	state = eState::eWait;
	// 基本的なアニメーションの再生
	mDefaultAnimation(state);
	
	// 移動に使う値のを取得
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0, m_cameraRotation._y, 0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

	// 壁に当たっているかの判定
	if (m_isHitWall){
		Vector3 revision = m_prevPosition.Normalize();
		if (m_sign == '+'){
			m_playerTransform._translation = m_prevPosition + Vector3(revision._x, 0, revision._z);
		}
		else if (m_sign == '-'){
			m_playerTransform._translation = m_prevPosition - Vector3(revision._x, 0, revision._z);
		}
		
		m_isHitWall = false; //フラグをオフにする
	
	}
	else{
		// カメラの回転行列を掛け合わせて、カメラの向きと進行方向を一致させる
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		m_playerTransform._translation += translation;
	}

	// 移動処理
	m_charaEntity.mGearMove(m_topGear, m_playerTransform._translation);

	// 回転処理
	m_charaEntity.mGearRotation(m_topGear, m_topGear, m_playerTransform._rotation);

	// コライダーの更新処理
	mUpdateBodyCollider(m_topGear->_pGear->property._transform);

	return;
}

/*
キーを読み込む
返りは今のところtransform
*/
std::pair<Transform, Vector3> FieldPlayer::mReadKey(const float timeScale, char& sign){

	std::pair<Transform, Vector3> output;

	/* 奥行の移動(Z軸)	*/
	if (GameController::GetKey().IsKeyDown('W')||GameController::GetJoypad().IsButtonDown(eJoyButton::eUp)){
		output.first._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
		output.first._rotation._y = 0;
		sign = '-';
	}
	else if (GameController::GetKey().IsKeyDown('S') || GameController::GetJoypad().IsButtonDown(eJoyButton::eDown)){
		output.first._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y = 180;
		sign = '+';
	}

	/* 横の移動(X軸)	*/
	if (GameController::GetKey().IsKeyDown('D') || GameController::GetJoypad().IsButtonDown(eJoyButton::eRight)){
		output.first._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
		output.first._rotation._y = 90;
		sign = '+';
	}
	else if (GameController::GetKey().IsKeyDown('A') || GameController::GetJoypad().IsButtonDown(eJoyButton::eLeft)){
		output.first._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y = 270;
		sign = '-';
	}

	/*	カメラの回転	*/	
	if (GameController::GetKey().IsKeyDown(VK_RIGHT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRB1)){
		output.second._y += (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}
	else if (GameController::GetKey().IsKeyDown(VK_LEFT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eLB1)){
		output.second._y -= (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	return output;
}

//
void FieldPlayer::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_topGear)return;
	m_playerView.Render();
	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_topGear, modelShader, colliderShader);

	return;
}

//
aetherClass::ViewCamera *FieldPlayer::mGetView(){
	return &m_playerView;
}

std::shared_ptr<Cube> FieldPlayer::mGetBodyColldier(){
	return m_pBodyCollider;
}

/*
ギア系の初期化をまとめたもの

*/
bool FieldPlayer::mInitializeGearFrame(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){


	//
	// 連想配列に登録
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eBody, gearFrame->m_pBody);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eWaist, gearFrame->m_pWaist);

	// 左
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftArm, gearFrame->m_pLeftArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftFoot, gearFrame->m_pLeftFoot);

	// 右
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightArm, gearFrame->m_pRightArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightFoot, gearFrame->m_pRightFoot);
	return true;
}

/*
	エディターから読み取り
*/
bool FieldPlayer::mLoadProperty(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
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
		if (index->_name == "LeftArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftArm, index);
		}

		/*	右上半身	*/
		if (index->_name == "RightArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightArm, index);
		}

		if (index->_name == "RightFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightFoot, index);
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
void FieldPlayer::mInitialPlayerView(CameraValue input){
	// カメラの初期化
	m_playerView.property._translation = input._position;
	m_playerView.property._rotation = input._rotation;

	// カメラのオフセットの設定
	m_cameraOffset._translation = m_playerView.property._translation + m_playerTransform._translation+Vector3(0,0,-50);
	m_cameraOffset._rotation = m_playerView.property._rotation;

	return;
}

/*
	コライダーの初期化用
*/

void FieldPlayer::mSetUpBodyCollider(std::shared_ptr<Cube>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset){
	
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
void FieldPlayer::mUpdateBodyCollider(Transform& transform){
	m_pBodyCollider->property._transform._translation = transform._translation + kColliderOffset;
	m_pBodyCollider->property._transform._rotation = transform._rotation;
	return;
}

/*
*/
void FieldPlayer::mRegisterAnimation(FieldPlayer::eState key, const int allFrame, std::string first, std::string last){
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
void FieldPlayer::mDefaultAnimation(FieldPlayer::eState& state){

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
		// 反転用、体だけ反転させる
		if (index._name == Gear::eType::eBody){
			animationTransform._scale._x = -1;
		}
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
	default:
		break;
	}
	

	return;
}

//
void FieldPlayer::mUpdateView(ViewCamera& view, Vector3& rotation, Vector3 lookAtPosition){

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

//
void FieldPlayer::mCheckCameraRotation(Vector3& rotation){
	// カメラ可動範囲の上限の確認
	if (rotation._x > kCameraRotationMaxX){
		rotation._x = kCameraRotationMaxX;
	}
	else if (rotation._x < kCameraRotationMinX){
		rotation._x = kCameraRotationMinX;
	}
	return;
}

// 壁に当たった時の処理
void FieldPlayer::mOnHitWall(){
	m_prevPosition = m_playerTransform._translation;
	m_isHitWall = true;
	return;
}

//
void FieldPlayer::mSetFieldNumber(const int number){
	m_fieldNumber = number;
}

//
int FieldPlayer::mGetFieldNumber()const{
	return m_fieldNumber;
}

//
void FieldPlayer::mSetTransform(Transform trans){
	m_playerTransform = trans;
	m_cameraRotation = m_playerTransform._rotation;
}

//
Transform FieldPlayer::mGetTransform(){
	return m_playerTransform;
}