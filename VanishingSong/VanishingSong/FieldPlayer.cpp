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

	// �����ʒu�̐ݒ�
	m_model = model;
	m_model->SetCamera(&m_playerView);
	m_model->property._transform = trans;

	WorldReader read;
	read.Load("data\\Player\\FieldCamera.aether");
	mInitialPlayerView(read.GetInputWorldInfo()._camera, m_model->property._transform._rotation);
	read.UnLoad();

	// �R���C�_�[�̏�����
	mSetUpBodyCollider(m_pBodyCollider, m_model->property._transform._translation, kColliderOffset);
	
	m_isHitWall = false;
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
	Transform playerTransform;
	if (!isWait){
		getKeyValues = mReadKey(timeScale);
	}

	// �J�����̏���
	m_cameraRotation += getKeyValues.second;
	mUpdateView(m_playerView, m_cameraRotation, m_model->property._transform._translation);

	// �ړ��������
	FieldPlayer::eState state;
	if (getKeyValues.first._translation == kVector3Zero){
		playerTransform._rotation._y = m_prevRotationY;
		state = eState::eWait;
	}
	else{
		// ���������ς���
		playerTransform._rotation._y = getKeyValues.first._rotation._y + m_cameraRotation._y;
		state = eState::eMove;
	}
	state = eState::eWait;
	
	// �ړ��Ɏg���l�̂��擾
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0, m_cameraRotation._y, 0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

	// �ǂɓ������Ă��邩�̔���
	if (m_isHitWall){

		// �J�����̉�]�s����|�����킹�āA�J�����̌����Ɛi�s��������v������
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		playerTransform._translation -= Vector3(10, 0, 10)-translation;
		m_isHitWall = false; //�t���O���I�t�ɂ���
	}
	else{
		// �J�����̉�]�s����|�����킹�āA�J�����̌����Ɛi�s��������v������
		Vector3 translation = getKeyValues.first._translation.TransformCoordNormal(rotationMatrix);
		playerTransform._translation += translation;
	}

	// �ړ�����
	m_model->property._transform._translation += playerTransform._translation;
	// ��]����

	m_model->property._transform._rotation = playerTransform._rotation;
	// �R���C�_�[�̍X�V����
	mUpdateBodyCollider(m_model->property._transform);
	m_prevRotationY = m_model->property._transform._rotation._y;
	m_prevVector = playerTransform._translation;
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
		output.first._rotation._y += 0;
	}
	else if (GameController::GetKey().IsKeyDown('S') || GameController::GetJoypad().IsButtonDown(eJoyButton::eDown)){
		output.first._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y += 180;
	}
	else
	/* ���̈ړ�(X��)	*/
	if (GameController::GetKey().IsKeyDown('D') || GameController::GetJoypad().IsButtonDown(eJoyButton::eRight)){
		output.first._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
		output.first._rotation._y += 90;
	}
	else if (GameController::GetKey().IsKeyDown('A') || GameController::GetJoypad().IsButtonDown(eJoyButton::eLeft)){
		output.first._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
		output.first._rotation._y += 270;
	}

	/*	�J�����̉�]	*/	
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
	collider->property._transform._scale = 10;
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
	return;
}


/*
	�R���C�_�[�̍X�V�p
*/
void FieldPlayer::mUpdateBodyCollider(Transform& transform){
	m_pBodyCollider->property._transform._translation = transform._translation + kColliderOffset;
//	m_pBodyCollider->property._transform._rotation = transform._rotation;
	return;
}

//
void FieldPlayer::mUpdateView(ViewCamera& view, Vector3& rotation, Vector3 lookAtPosition){

	// �J�����̃��Z�b�g�ꉞ�K�Ƃ���
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
