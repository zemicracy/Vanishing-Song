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
	const Vector3 kPlayerInitialY = Vector3(0, 20, 0);
	const float kDefaultMove = 100.0f;

}

FieldPlayer::~FieldPlayer()
{
	mFinalize();
}

//
bool FieldPlayer::mInitialize(){
	bool result;
	mFinalize();
	auto gearFrame = Singleton<ResourceManager>::GetInstance().mGetPlayerHash(eMusical::eBlue);
	// �M�A�n�̏������p
	result = mInitializeGearFrame(gearFrame, &m_playerView);
	if (!result){
		return false;
	}

	// �p�[�c�̏����ʒu
	result = mLoadProperty(gearFrame, "data\\Player\\Stay.aether");

	if (!result){
		return false;
	}
	
	// �R���C�_�[�̏�����
	mSetUpBodyCollider(m_pBodyCollider, m_topGear->_pGear->property._transform._translation, kColliderOffset);

	/*	��{�I�ȃA�j���[�V�����̓o�^	*/
	mRegisterAnimation(FieldPlayer::eState::eMove, kMoveAnimationFrame, "data\\Player\\Stay.aether", "data\\Player\\Move.aether");
	mRegisterAnimation(FieldPlayer::eState::eWait, kWaitAnimationFrame, "data\\Player\\Stay.aether", "data\\Player\\Wait.aether");

	m_initialTransform = m_playerTransform;
	m_isHitWall = false;
	
	m_charaEntity.SetCamera(m_topGear, &m_playerView);
	return true;
}

/*
�������
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
�v���C���[�̍X�V����
*/
void FieldPlayer::mUpdate(const float timeScale,const bool isWait){

	// �L�[�̏������擾
	KeyValues getKeyValues;
		if (!isWait){
		
			getKeyValues = mReadKey(timeScale);
		}
	// �J�����̏���
	m_cameraRotation += getKeyValues._cameraRotation;

	mUpdateView(m_playerView, m_cameraRotation, m_topGear->_pGear->property._transform._translation);

	// ���������ς���
	m_playerTransform._rotation._y += getKeyValues._cameraRotation._y;

	// �ړ��������
	FieldPlayer::eState state;
	if (getKeyValues._transform._translation == kVector3Zero){
		state = eState::eWait;
	}
	else{
		state = eState::eMove;
	}

	// ��{�I�ȃA�j���[�V�����̍Đ�
	mDefaultAnimation(state);

	// �ړ��Ɏg���l�̂��擾
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0, m_cameraRotation._y, 0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

	// �ǂɓ������Ă��邩�̔���
	if (m_isHitWall){
		Vector3 revision = m_prevPosition;
		m_playerTransform._translation = m_playerTransform._translation - Vector3(revision._x, 0, revision._z);
		m_isHitWall = false; //�t���O���I�t�ɂ���

	}
	else{
		// �J�����̉�]�s����|�����킹�āA�J�����̌����Ɛi�s��������v������
		Vector3 translation = getKeyValues._transform._translation.TransformCoordNormal(rotationMatrix);
		m_playerTransform._translation += translation;
	}

	// �ړ�����
	m_charaEntity.mGearMove(m_topGear, m_playerTransform._translation);

	// ��]����
	m_charaEntity.mGearRotation(m_topGear, m_topGear, m_playerTransform._rotation);

	// �R���C�_�[�̍X�V����
	mUpdateBodyCollider(m_topGear->_pGear->property._transform);

	return;
}

/*
�L�[��ǂݍ���
�Ԃ�͍��̂Ƃ���transform
*/
FieldPlayer::KeyValues FieldPlayer::mReadKey(const float timeScale){

	KeyValues output;

	/* ���s�̈ړ�(Z��)	*/
	if (GameController::GetKey().IsKeyDown('W')||GameController::GetJoypad().IsButtonDown(eJoyButton::eUp)){
		output._transform._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
	}
	else if (GameController::GetKey().IsKeyDown('S') || GameController::GetJoypad().IsButtonDown(eJoyButton::eDown)){
		output._transform._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	/* ���̈ړ�(X��)	*/
	if (GameController::GetKey().IsKeyDown('D') || GameController::GetJoypad().IsButtonDown(eJoyButton::eRight)){
		output._transform._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
	}
	else if (GameController::GetKey().IsKeyDown('A') || GameController::GetJoypad().IsButtonDown(eJoyButton::eLeft)){
		output._transform._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	/*	�J�����̉�]	*/	
	if (GameController::GetKey().IsKeyDown(VK_RIGHT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eRB1)){
		output._cameraRotation._y += (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}
	else if (GameController::GetKey().IsKeyDown(VK_LEFT) || GameController::GetJoypad().IsButtonDown(eJoyButton::eLB1)){
		output._cameraRotation._y -= (float)(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	return output;
}

//
void FieldPlayer::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_topGear)return;
	m_playerView.Render();
	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
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
�M�A�n�̏��������܂Ƃ߂�����

*/
bool FieldPlayer::mInitializeGearFrame(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){


	//
	// �A�z�z��ɓo�^
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eBody, gearFrame->m_pBody);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eWaist, gearFrame->m_pWaist);

	// ��
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftHand, gearFrame->m_pLeftHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerLeg, gearFrame->m_pLeftLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperArm, gearFrame->m_pLeftUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperLeg, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftFoot, gearFrame->m_pLeftFoot);
	// �E
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightHand, gearFrame->m_pRightHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerLeg, gearFrame->m_pRightLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperArm, gearFrame->m_pRightUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperLeg, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightFoot, gearFrame->m_pRightFoot);
	return true;
}

/*
	�G�f�B�^�[����ǂݎ��
*/
bool FieldPlayer::mLoadProperty(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
	WorldReader read;
	bool result = read.Load(modelDataFile.c_str());
	if (!result)
	{
		Debug::mErrorPrint("�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", __FILE__, __LINE__);
		return false;
	}
	m_topGear = gearFrame->m_pBody;
	for (auto index : read.GetInputWorldInfo()._object){

		/*	��	*/
		if (index->_name == "Body"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pWaist, index);
		}

		/*	���㔼�g*/
		if (index->_name == "LeftUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftUpperArm, index);
		}

		if (index->_name == "LeftLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftLowerArm, index);
		}

		if (index->_name == "LeftHand"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftHand, index);
		}

		/*	�E�㔼�g	*/
		if (index->_name == "RightUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightUpperArm, index);
		}

		if (index->_name == "RightLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightLowerArm, index);
		}

		if (index->_name == "RightHand"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightHand, index);
		}

		/*	�E��	*/
		if (index->_name == "RightUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightUpperLeg, index);
		}

		if (index->_name == "RightLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightLowerLeg, index);
		}

		if (index->_name == "RightFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightFoot, index);
		}

		/*	����	*/
		if (index->_name == "LeftUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftUpperLeg, index);
		}

		if (index->_name == "LeftLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftLowerLeg, index);
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
	�J�����̏�����
*/
void FieldPlayer::mInitialPlayerView(CameraValue input){
	// �J�����̏�����
	m_playerView.property._translation = input._position;
	m_playerView.property._rotation = input._rotation;

	// �����ʒu�̐ݒ�
	m_playerTransform._translation = kPlayerInitialY;

	// �J�����̃I�t�Z�b�g�̐ݒ�
	m_cameraOffset._translation = m_playerView.property._translation + m_playerTransform._translation+Vector3(0,0,-50);
	m_cameraOffset._rotation = m_playerView.property._rotation;

	return;
}

/*
	�R���C�_�[�̏������p
*/
void FieldPlayer::mSetUpBodyCollider(std::shared_ptr<aetherClass::Cube>& collider, Vector3 original, Vector3 offset){
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
	m_pBodyCollider->property._transform._rotation = transform._rotation;

	return;
}

/*
*/
void FieldPlayer::mRegisterAnimation(FieldPlayer::eState key, const int allFrame, std::string first, std::string last){
	AnimationFrame animation;
	bool result = false;

	// ���ɂ���o�^�ς݂Ȃ牽�����Ȃ�
	if (m_defaultAnimation.find(key) != m_defaultAnimation.end()) return;

	result = m_charaEntity.mLoadAnimation(animation._animation, first, last);
	if (!result)
	{
		Debug::mErrorPrint("�ǂݍ��ݎ��s", __FILE__, __LINE__);
		return;
	}
	animation._animationFrame = allFrame;

	// �o�^
	m_defaultAnimation.insert(std::make_pair(key, animation));

	return;
}

/*
	��{�I�ȃA�j���[�V�����̍Đ�
*/
void FieldPlayer::mDefaultAnimation(FieldPlayer::eState& state){

	// �O��ƈႤ�Ƃ��͍X�V
	if (m_prevState != state){
		m_actionCount._defaultFrame = NULL;
		m_prevState = state;
	}

	// �ݒ肳��Ă��Ȃ��ꍇ�������Ȃ�
	if (m_defaultAnimation.find(state) == m_defaultAnimation.end()) return;

	Transform animationTransform;

	/*	�A�j���[�V�������s����	*/
	const int allFrame = m_defaultAnimation[state]._animationFrame;

	for (auto index : m_defaultAnimation[state]._animation)
	{
		// ��Ԃ̒l���擾
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end,allFrame , m_actionCount._defaultFrame);

		// �A�j���[�V�����̓K�p
		if (m_pGearHash.find(index._name) != m_pGearHash.end()){
			m_pGearHash[index._name]->_pGear->property._transform = animationTransform;
		}
	}

	switch (state)
	{
		case eState::eWait:
			/*	�J�E���^�[�̏�Ԃ�؂�ւ���	*/
			if (m_actionCount._defaultFrame > kWaitAnimationFrame){
				m_actionCount._changeDefaultFrame = true;
			}
			else if (m_actionCount._defaultFrame < kZeroPoint){
				m_actionCount._changeDefaultFrame = false;
			}

			// �J�E���^�[�̏�Ԃɍ��킹�ăt���[���J�E���g�̍X�V
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
	m_prevPosition = m_playerTransform._translation.Normalize();
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