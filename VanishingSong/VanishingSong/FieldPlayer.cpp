#include "FieldPlayer.h"
#include "Debug.h"
#include "Const.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
#include <Windows.h>
#include <Physics.h>
#include "ResourceManager.h"
using namespace aetherClass;
using namespace aetherFunction;
namespace{
	const float kCameraRotationMaxX = -5.0f;
	const float kCameraRotationMinX = -20.0f;
	const Vector3 kColliderOffset = Vector3(0, 15, 0); 
	const float kWalk = 30.0f;
	const float kRun = 150.f;
	const float kCameraRotation = 150.0f;
}

FieldPlayer::~FieldPlayer()
{
	mFinalize();
}

//
bool FieldPlayer::mInitialize(std::shared_ptr<FbxModel> model, Transform trans){
	mFinalize();
	m_playerView = std::make_shared<aetherClass::ViewCamera>();
	// 初期位置の設定
	m_model = model;
	m_model->SetCamera(m_playerView.get());
	m_transform = trans;
	

	WorldReader read;
	read.Load("data\\Field\\field_camera",true);
	mInitialPlayerView(read.GetInputWorldInfo()._camera, m_model->property._transform._rotation);
	read.UnLoad();
	m_cameraRotation = trans._rotation;

	// コライダーの初期化
	mSetUpBodyCollider(m_pBodyCollider, m_model->property._transform._translation, kColliderOffset);

	m_pSphereCollider = std::make_shared<Sphere>(10, 10);
	m_pSphereCollider->Initialize();
	m_pSphereCollider->property._transform._translation = m_model->property._transform._translation + kColliderOffset;
	m_pSphereCollider->property._transform._scale = Vector3(10, 10, 10);
	m_pSphereCollider->property._color = Color(1, 0, 0, 0.5);
	m_pSphereCollider->SetCamera(m_playerView.get());

	m_animationName.insert(std::make_pair(eState::eRun, "run"));
	m_animationName.insert(std::make_pair(eState::eWalk, "walk"));
	m_animationName.insert(std::make_pair(eState::eWait, "wait"));
	m_isHitWall = false;
	m_isHitObject = false;
	m_preveAnimtionName = "null";
	m_animtationCount = NULL;
	m_isRun = false;
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
void FieldPlayer::mUpdate(const float timeScale, const bool isWait){

	// キーの処理を取得
	std::pair<Transform, Vector3> getKeyValues;

	if (!isWait){
		
		getKeyValues = mReadKey(timeScale);
	}

	// カメラの処理
	m_cameraRotation += getKeyValues.second;
	mUpdateView(m_playerView.get(), m_cameraRotation, m_model->property._transform._translation);

	// 移動があれば
	FieldPlayer::eState state;
	if (getKeyValues.first._translation == kVector3Zero){
		state = eState::eWait;
	}
	else if (m_isRun){
		// 向きだけ変える
		m_transform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eRun;
	}
	else{
		m_transform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eWalk;
	}

	m_prevState = state;
	// 移動に使う値のを取得
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0, m_cameraRotation._y, 0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

	// 壁に当たっているかの判定
	if (m_isHitWall){
		// カメラの回転行列を掛け合わせて、カメラの向きと進行方向を一致させる
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		m_transform._translation += translation;
		m_pBodyCollider->property._transform._translation = m_transform._translation;
		m_pSphereCollider->property._transform._translation = m_transform._translation;
		if (m_isHitObject){
			Vector3 hitPos;
			if (m_hitObject&&ColliderBoxSphere(*m_pSphereCollider, *m_hitObject, &hitPos)){
				m_transform._translation = m_prevPosition - Vector3(hitPos._x,0,hitPos._z);
			}
		}
		else if (m_isHitWall){
			if (m_hitWall&&CollideBoxOBB(*m_pBodyCollider, *m_hitWall)){
				Vector3 trans = Vector3(translation._x / translation._x, 0, translation._z / translation._z);
				m_transform._translation = m_prevPosition;
			}
		}
		
		//フラグをオフにする
		m_isHitObject = false;
		m_isHitWall = false; 
	}
	else{
		// カメラの回転行列を掛け合わせて、カメラの向きと進行方向を一致させる
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		m_transform._translation += translation;
	}


	m_model->property._transform._translation = m_transform._translation;
	m_model->property._transform._rotation = m_transform._rotation;

	// コライダーの更新処理
	mUpdateBodyCollider(m_transform);


	std::string animationName;
	animationName = m_animationName.at(state);
	// アニメーションの更新
	if (m_preveAnimtionName != animationName){
		m_animtationCount = NULL;
		m_preveAnimtionName = animationName;
	}

	if (m_animtationCount > m_model->GetKeyframeCount(m_preveAnimtionName) - 1){
		m_animtationCount = 0;
	}

	m_model->KeyframeUpdate(m_preveAnimtionName, m_animtationCount);
	m_animtationCount += 1;
	return;
}

/*
キーを読み込む
返りは今のところtransform
*/
std::pair<Transform, Vector3> FieldPlayer::mReadKey(const float timeScale){

	std::pair<Transform, Vector3> output;

	if (GameController::GetKey().IsKeyDown(VK_SHIFT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eLB1)){
		/* 奥行の移動(Z軸)	*/
		if (GameController::GetKey().IsKeyDown('W') || GameController::GetJoypad().IsButtonDown(eJoyButton::eUp)){
			output.first._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kRun;
			output.first._rotation._y = 0;
			m_isRun = true;
		}
		else if (GameController::GetKey().IsKeyDown('S') || GameController::GetJoypad().IsButtonDown(eJoyButton::eDown)){
			output.first._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kRun);
			output.first._rotation._y = 180;
			m_isRun = true;
		}
		else
			/* 横の移動(X軸)	*/
			if (GameController::GetKey().IsKeyDown('D') || GameController::GetJoypad().IsButtonDown(eJoyButton::eRight)){
				output.first._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kRun;
				output.first._rotation._y = 90;
				m_isRun = true;
			}
			else if (GameController::GetKey().IsKeyDown('A') || GameController::GetJoypad().IsButtonDown(eJoyButton::eLeft)){
				output.first._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kRun);
				output.first._rotation._y = 270;
				m_isRun = true;
			}
	}
	else{
		/* 奥行の移動(Z軸)	*/
		if (GameController::GetKey().IsKeyDown('W') || GameController::GetJoypad().IsButtonDown(eJoyButton::eUp)){
			output.first._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kWalk;
			output.first._rotation._y = 0;
			m_isRun = false;
		}
		else if (GameController::GetKey().IsKeyDown('S') || GameController::GetJoypad().IsButtonDown(eJoyButton::eDown)){
			output.first._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kWalk);
			output.first._rotation._y = 180;
			m_isRun = false;
		}
		else
			/* 横の移動(X軸)	*/
			if (GameController::GetKey().IsKeyDown('D') || GameController::GetJoypad().IsButtonDown(eJoyButton::eRight)){
				output.first._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kWalk;
				output.first._rotation._y = 90;
				m_isRun = false;
			}
			else if (GameController::GetKey().IsKeyDown('A') || GameController::GetJoypad().IsButtonDown(eJoyButton::eLeft)){
				output.first._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kWalk);
				output.first._rotation._y = 270;
				m_isRun = false;
			}
	}
	

	/*	カメラの回転	*/	
	if (GameController::GetKey().IsKeyDown(VK_RIGHT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRRight)){
		output.second._y += (float)(GameClock::GetDeltaTime()*timeScale*kCameraRotation);
	}
	else if (GameController::GetKey().IsKeyDown(VK_LEFT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRLeft)){
		output.second._y -= (float)(GameClock::GetDeltaTime()*timeScale*kCameraRotation);
	}else
	if (GameController::GetKey().IsKeyDown(VK_UP) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRUp)){
		output.second._x += (float)(GameClock::GetDeltaTime()*timeScale*kCameraRotation);
	}
	else if (GameController::GetKey().IsKeyDown(VK_DOWN) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRDown)){
		output.second._x -= (float)(GameClock::GetDeltaTime()*timeScale*kCameraRotation);
	}

	return output;
}

//
void FieldPlayer::mRender(ShaderBase* modelShader,ShaderBase* colliderShader){

	m_playerView->Render();
	if (!m_model)return;

	m_model->KeyframeAnimationRender(modelShader);
	return;
}
/*
	カメラの初期化
*/
void FieldPlayer::mInitialPlayerView(CameraValue input, Vector3 rotation){
	// カメラの初期化
	m_playerView->property._translation = input._position;
	m_playerView->property._rotation = input._rotation;
	
	// カメラのオフセットの設定
	m_cameraOffset._translation = m_playerView->property._translation;
	m_cameraOffset._rotation = m_playerView->property._rotation;

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
	collider->property._transform._scale = Vector3(5,10,5);
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(m_playerView.get());
	return;
}


/*
	コライダーの更新用
*/
void FieldPlayer::mUpdateBodyCollider(Transform& transform){
	m_pBodyCollider->property._transform._translation = transform._translation + kColliderOffset;
	m_pBodyCollider->property._transform._rotation = transform._rotation;

	m_pSphereCollider->property._transform._translation = transform._translation + kColliderOffset;
	m_pSphereCollider->property._transform._rotation = transform._rotation;

	return;
}

//
void FieldPlayer::mUpdateView(ViewCamera* view, Vector3& rotation, Vector3 lookAtPosition){

	// カメラのリセット一応階といた
	mCheckCameraRotation(rotation);
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0,rotation._y,0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);
	Vector3 position = m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	view->property._translation = Vector3(lookAtPosition._x,NULL,lookAtPosition._z) + position;
	view->property._rotation = rotation + m_cameraOffset._rotation;

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
void FieldPlayer::mOnHitWall(aetherClass::ModelBase* hitObject){
	m_prevPosition = m_transform._translation;
	m_hitWall = hitObject;
	m_isHitWall = true;
	m_isHitObject = false;
	return;
}

//
void FieldPlayer::mOnHitObject(aetherClass::Sphere* hitObject){
	m_prevPosition = m_transform._translation;
	m_hitObject = hitObject;
	m_isHitWall = true;
	m_isHitObject = true;

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
std::shared_ptr<ViewCamera>& FieldPlayer::mGetView(){
	return m_playerView;
}

std::shared_ptr<Cube>& FieldPlayer::mGetBodyColldier(){
	return m_pBodyCollider;
}

//
std::shared_ptr<Sphere>& FieldPlayer::mGetSphereColldier(){
	return m_pSphereCollider;
}

void FieldPlayer::mFaceToTalk(Vector3 pos){
	m_charaEntity.mFaceToObject(m_model, pos);
	m_transform._rotation = m_model->property._transform._rotation;
}