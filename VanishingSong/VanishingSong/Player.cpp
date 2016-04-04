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


bool Player::mInitialize(ViewCamera* camera){

	mFinalize();

	// �M�A�n�̏������p
	mInitializeGear(m_pGearFrame, camera);

	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[���f�o�b�O���[�h�ł�");
	}
	return true;
}


/*
	�v���C���[�̍X�V����
*/
void Player::mUpdate(const float timeScale){
	
	// �ړ��Ɏg���l�̂��擾
	Transform transform = mReadKey(timeScale);

	// ���ۂ̈ړ�����
	m_charaEntity.mGearMove(m_pTopGear, transform._translation);

	// ��]����
	// �܂�����ȉ�]�����ɂȂ��Ă��Ȃ�
	m_charaEntity.mGearRotation(m_pTopGear, transform._rotation);
	return;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){
	
	if (!m_pTopGear)return;

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	m_charaEntity.mGearRender(m_pTopGear, modelShader, colliderShader);

	return;
}

//
eActionType Player::mAction(std::shared_ptr<ActionCommand> command,const float timeScale){

	// ������s���A�N�V�������擾
	m_status._nowAction = command->mGetType();

	if (m_status._nowAction != m_prevAction){
		// �O��ƈႦ�Ύ��s����0�ɂ���
		m_actionCount = kZeroPoint;
	}

	// �A�N�V�����̎��s
	command->mAction(m_pGearFrame, timeScale, m_actionCount);
	
	m_actionCount += 1;

	// ��Ԃ��㏑��
	m_prevAction = m_status._nowAction;
	
	return m_prevAction;
}

/*
	�s�������A�N�V������o�^
	�������F�A�N�V�����̃^�C�v
	�������F���Ԗڂ̃A�N�V������
*/
void Player::mAddPrevActionCmmand(eActionType action,const int id){
	m_status._prevActionList[id] = action;

	return;
}

/*
	NULL�Ŗ��ߐs����
*/
void Player::mResetPrevActionList(){
	m_status._prevActionList.fill(eActionType::eNull);

	return;
}

/*
	�������
*/
void Player::mFinalize(){
	if (m_pGearFrame)
	{
		m_pGearFrame->Release();
		m_pGearFrame.reset();
		m_pGearFrame = nullptr;
	}

	if (m_pTopGear)
	{
		m_pTopGear->Release();
		m_pTopGear.reset();
		m_pTopGear = nullptr;
	}

	if (m_pActionCommand)
	{
		m_pActionCommand.reset();
		m_pActionCommand = nullptr;
	}
	
	m_status.Reset();

	m_prevAction = eActionType::eNull;
	
	m_actionCount = NULL;
	return;
}

/*
	�M�A�n�̏��������܂Ƃ߂�����

*/
bool Player::mInitializeGear(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){
	gearFrame = std::make_shared<GearFrame>();

	// �̂̃p�[�c
	gearFrame->m_pBody = m_charaEntity.mSetUpGear("null", Gear::eType::eBody, camera);

	// ���̃p�[�c
	gearFrame->m_pWaist = m_charaEntity.mSetUpGear("null", Gear::eType::eWaist, camera);

	// �r�̃p�[�c
	gearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperArm, camera);
	gearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperArm, camera);
	gearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerArm, camera);
	gearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerArm, camera);

	// ��̃p�[�c
	gearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftHand, camera);
	gearFrame->m_pRightHand = m_charaEntity.mSetUpGear("null", Gear::eType::eRightHand, camera);

	// ���̃p�[�c
	gearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperLeg, camera);
	gearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperLeg, camera);
	gearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerLeg, camera);
	gearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerLeg, camera);

	// �p�[�c�̏����ʒu
	WorldReader read;
	read.Load("data\\Player.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "Body"){
			gearFrame->m_pBody->_pColider->property._transform = index->_transform;
		}

		if (index->_name == "LeftArm"){
			gearFrame->m_pLeftUpperArm->_pColider->property._transform = index->_transform;
		}

		if (index->_name == "LeftLowerArm"){
			gearFrame->m_pLeftLowerArm->_pColider->property._transform = index->_transform;
		}
	}
	read.UnLoad();

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pLeftUpperArm);

	// �E�r�̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerArm, gearFrame->m_pRightHand);

	// ���r�̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerArm, gearFrame->m_pLeftHand);

	// �E���̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperLeg, gearFrame->m_pRightLowerLeg);

	// �����̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperLeg, gearFrame->m_pLeftLowerLeg);

	// �ŏ�ʂɓ�����p�[�c�̐ݒ�
	m_pTopGear = gearFrame->m_pBody;

	return true;
}

/*
�L�[��ǂݍ���
�Ԃ�͍��̂Ƃ���transform
*/
Transform Player::mReadKey(const float timeScale){

	Transform transform;

	// ���s�̈ړ�(Z��)
	if (GameController::GetKey().IsKeyDown('W')){
		transform._translation._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		transform._translation._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// ���̈ړ�(X��)
	if (GameController::GetKey().IsKeyDown('D')){
		transform._translation._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		transform._translation._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	// �L�������f�o�b�O���[�h����Ȃ��Ȃ炱���ŏI��
	if (!kCharaDebug) return transform;

	// ��]�p(Y��)
	if (GameController::GetKey().IsKeyDown('Q')){
		transform._rotation._y = GameClock::GetDeltaTime()*timeScale * 100;
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		transform._rotation._y = -(GameClock::GetDeltaTime()*timeScale * 100);
	}

	// �f�o�b�O�p
	Vector3 debug = m_pTopGear->_pGear->property._transform._translation;
	if (GameController::GetKey().KeyDownTrigger('P')){
		Debug::mPrint("---Player�̌��݂̍��W---");
		Debug::mPrint("X :" + std::to_string(debug._x));
		Debug::mPrint("Y :" + std::to_string(debug._y));
		Debug::mPrint("Z :" + std::to_string(debug._z));
		Debug::mPrint("------------------------");
	}

	return transform;
}
