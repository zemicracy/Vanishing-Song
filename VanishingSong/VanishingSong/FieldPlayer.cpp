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
	// �����ʒu�̐ݒ�
	m_model = model;
	m_model->SetCamera(m_playerView.get());
	m_transform = trans;
	

	WorldReader read;
	read.Load("data\\Field\\field_camera",true);
	mInitialPlayerView(read.GetInputWorldInfo()._camera, m_model->property._transform._rotation);
	read.UnLoad();
	m_cameraRotation = trans._rotation;

	// �R���C�_�[�̏�����
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
�������
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
�v���C���[�̍X�V����
*/
void FieldPlayer::mUpdate(const float timeScale, const bool isWait){

	// �L�[�̏������擾
	std::pair<Transform, Vector3> getKeyValues;

	if (!isWait){
		
		getKeyValues = mReadKey(timeScale);
	}

	// �J�����̏���
	m_cameraRotation += getKeyValues.second;
	mUpdateView(m_playerView.get(), m_cameraRotation, m_model->property._transform._translation);

	// �ړ��������
	FieldPlayer::eState state;
	if (getKeyValues.first._translation == kVector3Zero){
		state = eState::eWait;
	}
	else if (m_isRun){
		// ���������ς���
		m_transform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eRun;
	}
	else{
		m_transform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eWalk;
	}

	m_prevState = state;
	// �ړ��Ɏg���l�̂��擾
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0, m_cameraRotation._y, 0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

	// �ǂɓ������Ă��邩�̔���
	if (m_isHitWall){
		// �J�����̉�]�s����|�����킹�āA�J�����̌����Ɛi�s��������v������
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
		
		//�t���O���I�t�ɂ���
		m_isHitObject = false;
		m_isHitWall = false; 
	}
	else{
		// �J�����̉�]�s����|�����킹�āA�J�����̌����Ɛi�s��������v������
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		m_transform._translation += translation;
	}


	m_model->property._transform._translation = m_transform._translation;
	m_model->property._transform._rotation = m_transform._rotation;

	// �R���C�_�[�̍X�V����
	mUpdateBodyCollider(m_transform);


	std::string animationName;
	animationName = m_animationName.at(state);
	// �A�j���[�V�����̍X�V
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
�L�[��ǂݍ���
�Ԃ�͍��̂Ƃ���transform
*/
std::pair<Transform, Vector3> FieldPlayer::mReadKey(const float timeScale){

	std::pair<Transform, Vector3> output;

	if (GameController::GetKey().IsKeyDown(VK_SHIFT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eLB1)){
		/* ���s�̈ړ�(Z��)	*/
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
			/* ���̈ړ�(X��)	*/
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
		/* ���s�̈ړ�(Z��)	*/
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
			/* ���̈ړ�(X��)	*/
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
	

	/*	�J�����̉�]	*/	
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
	�J�����̏�����
*/
void FieldPlayer::mInitialPlayerView(CameraValue input, Vector3 rotation){
	// �J�����̏�����
	m_playerView->property._translation = input._position;
	m_playerView->property._rotation = input._rotation;
	
	// �J�����̃I�t�Z�b�g�̐ݒ�
	m_cameraOffset._translation = m_playerView->property._translation;
	m_cameraOffset._rotation = m_playerView->property._rotation;

	m_cameraRotation = rotation;
	return;
}

/*
	�R���C�_�[�̏������p
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
	�R���C�_�[�̍X�V�p
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

	// �J�����̃��Z�b�g�ꉞ�K�Ƃ���
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

	// �J�������͈͂̏���̊m�F
	if (rotation._x > kCameraRotationMaxX){
		rotation._x = kCameraRotationMaxX;
	}
	else if (rotation._x < kCameraRotationMinX){
		rotation._x = kCameraRotationMinX;
	}
	return;
}

// �ǂɓ����������̏���
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