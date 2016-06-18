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
	const float kDefaultMove = 100.0f;

}

FieldPlayer::~FieldPlayer()
{
	mFinalize();
}

//
bool FieldPlayer::mInitialize(std::shared_ptr<FbxModel> model, Transform trans){
	mFinalize();

	// �����ʒu�̐ݒ�
	m_model = model;
	m_model->SetCamera(&m_playerView);
	m_transform = trans;
	

	WorldReader read;
	read.Load("data\\Field\\field_camera.aether");
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
	m_pSphereCollider->SetCamera(&m_playerView);

	m_animationName.insert(std::make_pair(eState::eMove, "move"));
	m_animationName.insert(std::make_pair(eState::eWait, "wait"));
	m_isHitWall = false;
	m_isHitObject = false;
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
void FieldPlayer::mUpdate(const float timeScale,const bool isWait){

	// �L�[�̏������擾
	std::pair<Transform, Vector3> getKeyValues;

	if (!isWait){
		getKeyValues = mReadKey(timeScale);
	}

	// �J�����̏���
	m_cameraRotation += getKeyValues.second;
	mUpdateView(m_playerView, m_cameraRotation, m_model->property._transform._translation);

	// �ړ��������
	FieldPlayer::eState state;
	if (getKeyValues.first._translation == kVector3Zero){
		state = eState::eWait;
	}
	else{
		// ���������ς���
		m_transform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eMove;
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
				m_transform._translation = m_prevPosition - hitPos;
			}
		}
		else{
			if (m_hitWall&&CollideBoxOBB(*m_pBodyCollider, *m_hitWall)){
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
	Debug::mPrint("X :"+std::to_string(m_transform._translation._x));
	Debug::mPrint("Y :" + std::to_string(m_transform._translation._y));
	Debug::mPrint("Z :" + std::to_string(m_transform._translation._z));
	// �R���C�_�[�̍X�V����
	mUpdateBodyCollider(m_transform);

	// �A�j���[�V�����̍X�V
	m_model->KeyframeUpdate(m_model->GetKeyframeNameList(0), true);
	return;
}

/*
�L�[��ǂݍ���
�Ԃ�͍��̂Ƃ���transform
*/
std::pair<Transform, Vector3> FieldPlayer::mReadKey(const float timeScale){

	std::pair<Transform, Vector3> output;

	/* ���s�̈ړ�(Z��)	*/
	if (GameController::GetKey().IsKeyDown('W')||GameController::GetJoypad().IsButtonDown(eJoyButton::eUp)){
		output.first._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
		output.first._rotation._y = 0;
	}
	else if (GameController::GetKey().IsKeyDown('S') || GameController::GetJoypad().IsButtonDown(eJoyButton::eDown)){
		output.first._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y = 180;
	}
	else
	/* ���̈ړ�(X��)	*/
	if (GameController::GetKey().IsKeyDown('D') || GameController::GetJoypad().IsButtonDown(eJoyButton::eRight)){
		output.first._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
		output.first._rotation._y = 90;
	}
	else if (GameController::GetKey().IsKeyDown('A') || GameController::GetJoypad().IsButtonDown(eJoyButton::eLeft)){
		output.first._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y = 270;
	}

	/*	�J�����̉�]	*/	
	if (GameController::GetKey().IsKeyDown(VK_RIGHT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRRight)){
		output.second._y += (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}
	else if (GameController::GetKey().IsKeyDown(VK_LEFT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRLeft)){
		output.second._y -= (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}else
	if (GameController::GetKey().IsKeyDown(VK_UP) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRUp)){
		output.second._x += (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}
	else if (GameController::GetKey().IsKeyDown(VK_DOWN) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRDown)){
		output.second._x -= (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	return output;
}

//
void FieldPlayer::mRender(ShaderBase* modelShader,ShaderBase* colliderShader){

	m_playerView.Render();
	if (!m_model)return;

	m_model->KeyframeAnimationRender(modelShader);
	return;
}
/*
	�J�����̏�����
*/
void FieldPlayer::mInitialPlayerView(CameraValue input, Vector3 rotation){
	// �J�����̏�����
	m_playerView.property._translation = input._position;
	m_playerView.property._rotation = input._rotation;
	
	// �J�����̃I�t�Z�b�g�̐ݒ�
	m_cameraOffset._translation = m_playerView.property._translation;
	m_cameraOffset._rotation = m_playerView.property._rotation;

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
	collider->property._transform._scale = Vector3(5,10,10);
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
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
void FieldPlayer::mUpdateView(ViewCamera& view, Vector3& rotation, Vector3 lookAtPosition){

	// �J�����̃��Z�b�g�ꉞ�K�Ƃ���
	mCheckCameraRotation(rotation);
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0,rotation._y,0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);
	Vector3 position = m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	view.property._translation = Vector3(lookAtPosition._x,NULL,lookAtPosition._z) + position;
	view.property._rotation = rotation + m_cameraOffset._rotation;

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
ViewCamera* FieldPlayer::mGetView(){
	return &m_playerView;
}

std::shared_ptr<Cube>& FieldPlayer::mGetBodyColldier(){
	return m_pBodyCollider;
}

//
std::shared_ptr<Sphere>& FieldPlayer::mGetSphereColldier(){
	return m_pSphereCollider;
}
