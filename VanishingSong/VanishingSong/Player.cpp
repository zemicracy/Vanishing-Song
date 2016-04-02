#include "Player.h"
#include "Debug.h"
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
using namespace aetherClass;

Player::Player()
{
	m_pGearFrame = nullptr;
	m_actionHash.clear();
	m_pActionCommand = nullptr;
	
}


Player::~Player()
{
}


bool Player::mInitialize(ViewCamera* camera){
	m_pGearFrame = std::make_shared<GearFrame>();

	// �̂̃p�[�c
	m_pGearFrame->m_pBody = m_charaEntity.mSetUpGear("null", Gear::eType::eBody, camera);

	// ���̃p�[�c
	m_pGearFrame->m_pWaist = m_charaEntity.mSetUpGear("null", Gear::eType::eWaist, camera);

	//���̃p�[�c
	m_pGearFrame->m_pRightShoulder = m_charaEntity.mSetUpGear("null", Gear::eType::eRightShourlder, camera);
	m_pGearFrame->m_pLeftShoulder = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftShourlder, camera);

	// �r�̃p�[�c
	m_pGearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperArm, camera);
	m_pGearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperArm, camera);
	m_pGearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerArm, camera);
	m_pGearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerArm, camera);

	// ��̃p�[�c
	m_pGearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftHand, camera);
	m_pGearFrame->m_pRightHand = m_charaEntity.mSetUpGear("null", Gear::eType::eRightHand, camera);

	// ���̃p�[�c
	m_pGearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperLeg, camera);
	m_pGearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperLeg, camera);
	m_pGearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerLeg, camera);
	m_pGearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerLeg, camera);

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pRightShoulder);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pLeftShoulder);

	// �E�r�̐e�q�֌W
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightShoulder, m_pGearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightUpperArm, m_pGearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightLowerArm, m_pGearFrame->m_pRightHand);

	// ���r�̐e�q�֌W
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftShoulder, m_pGearFrame->m_pLeftUpperArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftUpperArm, m_pGearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftLowerArm, m_pGearFrame->m_pLeftHand);

	// �E���̐e�q�֌W
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightUpperLeg, m_pGearFrame->m_pRightLowerLeg);

	// �����̐e�q�֌W
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftUpperLeg, m_pGearFrame->m_pLeftLowerLeg);

	WorldReader read;
	read.Load("data\\Player.aether");
	for (auto index : read.GetInputWorldInfo()._object)
	{
		if (index->_name == "Body")
		{
			m_pGearFrame->m_pBody->_pColider->property._transform = index->_transform;
		}

		if (index->_name == "LeftArm")
		{
			m_pGearFrame->m_pLeftUpperArm->_pColider->property._transform = index->_transform;
		}

		if (index->_name == "LeftLowerArm")
		{
			m_pGearFrame->m_pLeftLowerArm->_pColider->property._transform = index->_transform;
		}
	}
	
	read.UnLoad();
	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[���f�o�b�O���[�h�ł�");
	}
	return true;
}

//
void Player::mUpdate(const float timeScale){
	
	// �ړ�����
	mReadKey(timeScale);

	return;
}

//
void Player::mReadKey(const float timeScale){
	Vector3 move = kVector3Zero;

	// ���s�̈ړ�(Z��)
	if (GameController::GetKey().IsKeyDown('W')){
		move._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		move._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// ���̈ړ�(X��)
	if (GameController::GetKey().IsKeyDown('D')){
		move._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		move._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	m_pGearFrame->m_pBody->_pColider->property._transform._translation += move;
	Vector3 hoge = m_pGearFrame->m_pBody->_pColider->property._transform._translation;
	
	if (kCharaDebug&&GameController::GetKey().KeyDownTrigger('P')){
		Debug::mPrint("---Player�̌��݂̍��W---");
		Debug::mPrint("X :" + std::to_string(hoge._x));
		Debug::mPrint("Y :" + std::to_string(hoge._y));
		Debug::mPrint("Z :" + std::to_string(hoge._z));
		Debug::mPrint("------------------------");
	}
	m_pGearFrame->m_pBody->_pColider->property._transform._translation += move;
}


//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){
	
	if (!m_pGearFrame->m_pBody)return;

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	m_charaEntity.mGearRender(m_pGearFrame->m_pBody, modelShader,colliderShader);
}

//
void Player::mAction(std::shared_ptr<ActionCommand> command){
	return;
}
