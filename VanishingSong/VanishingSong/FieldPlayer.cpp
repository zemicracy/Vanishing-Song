#include "FieldPlayer.h"
#include "Debug.h"
#include "Utility.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
#include <Windows.h>
using namespace aetherClass;

namespace{
	const int kWaitAnimationFrame = 60;
	const int kMoveAnimationFrame = 5;
	const float kCameraRotationMaxX = 15.0f;
	const float kCameraRotationMinX = -15.0f;
	const float kCameraY = 500.0f;
	const Vector3 kColliderOffset = Vector3(0, -5, 0); 
	const Vector3 kPlayerInitialY = Vector3(0, 20, 0);
	const Vector3 kBulletSpeed = Vector3(0, 0, 10);
	const float kDefaultMove = 100.0f;
	const float kDefaultMpHeal = 0.05f;
}

FieldPlayer::FieldPlayer()
{
	m_pGearFrame = nullptr;
	m_topGear = nullptr;
}


FieldPlayer::~FieldPlayer()
{
	mFinalize();
}

//
bool FieldPlayer::mInitialize(){
	bool result;
	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[ �f�o�b�O���[�h�ł�");
		Debug::mPrint("�v���C���[ ���������J�n���܂�");
	}

	mFinalize();

	// �M�A�n�̏������p
	result = mInitializeGearFrame(m_pGearFrame, &m_playerView);
	if (!result)
	{
		Debug::mErrorPrint("�M�A�̏������Ɏ��s", __FILE__, __LINE__);
		return false;
	}

	// �p�[�c�̏����ʒu
	result = mLoadProperty(m_pGearFrame, "data\\Player\\Stay.aether");

	if (!result)
	{
		Debug::mErrorPrint("Editor����̈ʒu���f�Ɏ��s", __FILE__, __LINE__);
		return false;
	}

	// �R���C�_�[�̏�����
	mSetUpBodyCollider(m_pBodyCollider, m_topGear->_pGear->property._transform._translation, kColliderOffset);

	/*	��{�I�ȃA�j���[�V�����̓o�^	*/
	mRegisterAnimation(FieldPlayer::eState::eMove, kMoveAnimationFrame, "data\\Player\\Stay.aether", "data\\Player\\Move.aether");
	mRegisterAnimation(FieldPlayer::eState::eWait, kWaitAnimationFrame, "data\\Player\\Stay.aether", "data\\Player\\Wait.aether");

	m_initialTransform = m_playerTransform;
	
	m_isHitWall = false;

	
	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[ �������I�����܂���");
	}
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
	
	if (m_pGearFrame)
	{
		m_pGearFrame->Release();
		m_pGearFrame.reset();
		m_pGearFrame = nullptr;
	}

	if (m_pBodyCollider)
	{
		m_pBodyCollider->Finalize();
		m_pBodyCollider.reset();
		m_pBodyCollider = nullptr;
	}
	m_prevState = eState::eNull;
	m_cameraRotation = kVector3Zero;
	return;
}


/*
�v���C���[�̍X�V����
*/
void FieldPlayer::mUpdate(const float timeScale){

	// �L�[�̏������擾
	KeyValues getKeyValues = mReadKey(timeScale);
	
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
	if (GameController::GetKey().IsKeyDown('W')){
		output._transform._translation._z = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		output._transform._translation._z = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	/* ���̈ړ�(X��)	*/
	if (GameController::GetKey().IsKeyDown('D')){
		output._transform._translation._x = (float)GameClock::GetDeltaTime()*timeScale*kDefaultMove;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		output._transform._translation._x = (float)-(GameClock::GetDeltaTime()*timeScale*kDefaultMove);
	}

	/*	�J�����̉�]	*/
	Vector2 mousePosition = GameController::GetMouse().GetMousePosition();

	/*	�R�}���h��I�[�_�[���X�g�̉ӏ��ȊO�݂̂ɔ�������*/
	if (mousePosition._y < kCameraY){
		if (GameController::GetMouse().IsRightButtonDown()){
			DirectXEntity directXEntity;
			gLockMouseCursor(directXEntity.GetWindowHandle(kWindowName), true);
			Vector2 cameraRotation = GameController::GetMouse().GetMouseMovement();
			cameraRotation /= (float)kAetherPI;
			output._cameraRotation._x += cameraRotation._y;
			output._cameraRotation._y += cameraRotation._x;

		}
	}

	return output;
}

//
void FieldPlayer::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_topGear)return;
	m_playerView.Render();
	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	m_charaEntity.mGearRender(m_topGear, modelShader, colliderShader);

	if (kCharaDebug)
	{
		//m_pBodyCollider->Render(colliderShader);
	}

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
	
	gearFrame = std::make_shared<GearFrame>();

	// �̂̃p�[�c
	gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Player\\body.fbx", Gear::eType::eBody, camera,"Model\\Player\\tex");

	// ���̃p�[�c
	gearFrame->m_pWaist = m_charaEntity.mSetUpGear("Model\\Player\\waist.fbx", Gear::eType::eWaist, camera, "Model\\Player\\tex");

	// �r�̃p�[�c
	gearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eLeftUpperArm, camera, "Model\\Player\\tex");
	gearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eRightUpperArm, camera, "Model\\Player\\tex");
	gearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eLeftLowerArm, camera, "Model\\Player\\tex");
	gearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eRightLowerArm, camera, "Model\\Player\\tex");

	// ��̃p�[�c
	gearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eLeftHand, camera, "Model\\Player\\tex");
	gearFrame->m_pRightHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eRightHand, camera, "Model\\Player\\tex");

	// ���̃p�[�c
	gearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eLeftUpperLeg, camera, "Model\\Player\\tex");
	gearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eRightUpperLeg, camera, "Model\\Player\\tex");
	gearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eLeftLowerLeg, camera, "Model\\Player\\tex");
	gearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eRightLowerLeg, camera, "Model\\Player\\tex");

	gearFrame->m_pLeftFoot= m_charaEntity.mSetUpGear("Model\\Player\\foot.fbx", Gear::eType::eLeftFoot, camera, "Model\\Player\\tex");
	gearFrame->m_pRightFoot= m_charaEntity.mSetUpGear("Model\\Player\\foot.fbx", Gear::eType::eRightFoot, camera, "Model\\Player\\tex");
	// �ŏ�ʂɓ�����p�[�c�̐ݒ�
	m_topGear = gearFrame->m_pBody;

	// ���ꂼ��̃p�[�c�Ƃ̐e�q�֌W�\�z
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pLeftUpperArm);

	// �E
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerArm, gearFrame->m_pRightHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperLeg, gearFrame->m_pRightLowerLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerLeg, gearFrame->m_pRightFoot);

	// ��
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerArm, gearFrame->m_pLeftHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperLeg, gearFrame->m_pLeftLowerLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerLeg, gearFrame->m_pLeftFoot);

	//
	// �A�z�z��ɓo�^
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eBody, m_pGearFrame->m_pBody);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eWaist, m_pGearFrame->m_pWaist);

	// ��
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftHand, m_pGearFrame->m_pLeftHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerArm, m_pGearFrame->m_pLeftLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerLeg, m_pGearFrame->m_pLeftLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperArm, m_pGearFrame->m_pLeftUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperLeg, m_pGearFrame->m_pLeftUpperLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftFoot, m_pGearFrame->m_pLeftFoot);
	// �E
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightHand, m_pGearFrame->m_pRightHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerArm, m_pGearFrame->m_pRightLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerLeg, m_pGearFrame->m_pRightLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperArm, m_pGearFrame->m_pRightUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperLeg, m_pGearFrame->m_pRightUpperLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightFoot, m_pGearFrame->m_pRightFoot);
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