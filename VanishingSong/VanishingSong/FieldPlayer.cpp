#include "FieldPlayer.h"
#include "Debug.h"
#include "Const.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
#include <Windows.h>
#include "ResourceManager.h"
using namespace aetherClass;

namespace{
	const float kCameraRotationMaxX = 15.0f;
	const float kCameraRotationMinX = -15.0f;
	const Vector3 kColliderOffset = Vector3(0, 15, 0); 
	const float kDefaultMove = 100.0f;

}

FieldPlayer::~FieldPlayer()
{
	mFinalize();
}

//
bool FieldPlayer::mInitialize(std::shared_ptr<FbxModel> model, Transform trans){
	mFinalize();

	// 初期位置の設定
	m_model = model;
	m_model->SetCamera(&m_playerView);
	m_model->property._transform = trans;

	WorldReader read;
	read.Load("data\\Player\\FieldCamera.aether");
	mInitialPlayerView(read.GetInputWorldInfo()._camera, m_model->property._transform._rotation);
	read.UnLoad();

	// コライダーの初期化
	mSetUpBodyCollider(m_pBodyCollider, m_model->property._transform._translation, kColliderOffset);
	
	m_isHitWall = false;
	return true;
}

/*
解放処理
*/
void FieldPlayer::mFinalize(){

	if (m_pBodyCollider)
	{
		m_pBodyCollider->Finalize();
		m_pBodyCollider.reset();
		m_pBodyCollider = nullptr;
	}	
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
	Transform playerTransform;
	if (!isWait){
		getKeyValues = mReadKey(timeScale);
	}

	// カメラの処理
	m_cameraRotation += getKeyValues.second;
	mUpdateView(m_playerView, m_cameraRotation, m_model->property._transform._translation);

	// 移動があれば
	FieldPlayer::eState state;
	if (getKeyValues.first._translation == kVector3Zero){
		playerTransform._rotation._y = m_prevRotationY;
		state = eState::eWait;
	}
	else{
		// 向きだけ変える
		playerTransform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eMove;
	}
	state = eState::eWait;
	
	// 移動に使う値のを取得
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0, m_cameraRotation._y, 0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

	// 壁に当たっているかの判定
	if (m_isHitWall){

		// カメラの回転行列を掛け合わせて、カメラの向きと進行方向を一致させる
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		playerTransform._translation -= Vector3(10, 0, 10)-translation;
		m_isHitWall = false; //フラグをオフにする
	}
	else{
		// カメラの回転行列を掛け合わせて、カメラの向きと進行方向を一致させる
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		playerTransform._translation += translation;
	}

	// 移動処理
	m_model->property._transform._translation += playerTransform._translation;
	// 回転処理

	m_model->property._transform._rotation = playerTransform._rotation;
	// コライダーの更新処理
	mUpdateBodyCollider(m_model->property._transform);
	m_prevRotationY = m_model->property._transform._rotation._y;
	m_prevVector = playerTransform._translation;
	return;
}

/*
キーを読み込む
返りは今のところtransform
*/
std::pair<Transform, Vector3> FieldPlayer::mReadKey(const float timeScale){

	std::pair<Transform, Vector3> output;

	/* 奥行の移動(Z軸)	*/
	if (GameController::GetKey().IsKeyDown('W')||GameController::GetJoypad().IsButtonDown(eJoyButton::eUp)){
		output.first._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
		output.first._rotation._y += 0;
	}
	else if (GameController::GetKey().IsKeyDown('S') || GameController::GetJoypad().IsButtonDown(eJoyButton::eDown)){
		output.first._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y += 180;
	}
	else
	/* 横の移動(X軸)	*/
	if (GameController::GetKey().IsKeyDown('D') || GameController::GetJoypad().IsButtonDown(eJoyButton::eRight)){
		output.first._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
		output.first._rotation._y += 90;
	}
	else if (GameController::GetKey().IsKeyDown('A') || GameController::GetJoypad().IsButtonDown(eJoyButton::eLeft)){
		output.first._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y += 270;
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
void FieldPlayer::mRender(ShaderBase* modelShader,ShaderBase* colliderShader){

	m_playerView.Render();
	if (!m_model)return;

	m_model->Render(modelShader);
	m_pBodyCollider->Render(colliderShader);
	return;
}
/*
	カメラの初期化
*/
void FieldPlayer::mInitialPlayerView(CameraValue input, Vector3 rotation){
	// カメラの初期化
	m_playerView.property._translation = input._position;
	m_playerView.property._rotation = input._rotation;
	
	// カメラのオフセットの設定
	m_cameraOffset._translation = m_playerView.property._translation;
	m_cameraOffset._rotation = m_playerView.property._rotation;

	m_cameraRotation = rotation;
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
//	m_pBodyCollider->property._transform._rotation = transform._rotation;
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
Transform FieldPlayer::mGetTransform(){
	return m_model->property._transform;
}

//
ViewCamera* FieldPlayer::mGetView(){
	return &m_playerView;
}

std::shared_ptr<Cube> FieldPlayer::mGetBodyColldier(){
	return m_pBodyCollider;
}
