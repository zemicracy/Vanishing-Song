#include "EnemyGround.h"
#include "EnemyMove.h"
#include "EnemyDie.h"
#include "EnemyWait.h"
#include <WorldReader.h>
#include "Debug.h"

using namespace aetherClass;

EnemyGround::EnemyGround(){

}

EnemyGround::~EnemyGround()
{
	mFinalize();
}

bool EnemyGround::mSetUp(){
	return true;
}


bool EnemyGround::mInitialize(ViewCamera* camera){

	//�`�悷�邩�ǂ���
	mGetProperty().m_isRender = false;
	mGetProperty()._penemy = std::make_shared<GearFrame>();

	//�ŏ��̍s���p�^�[��
	mGetCharaStatus()._nowAction = eActionType::eWait;

	// �̂̃p�[�c
	mGetProperty()._penemy->m_pBody = mGetCharaEntity().mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eBody, camera,"null");
	// ���̃p�[�c
	mGetProperty()._penemy->m_pWaist = mGetCharaEntity().mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eWaist, camera,"null");

	WorldReader read;
	read.Load("data\\Enemy.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			SetLoadModelValue(mGetProperty()._penemy->m_pBody, index);
			mGetProperty()._penemy->m_pBody->_pGear->property._transform._scale = 0.1;
		}

		if (index->_name == "West"){
			SetLoadModelValue(mGetProperty()._penemy->m_pWaist, index);
			mGetProperty()._penemy->m_pWaist->_pGear->property._transform._scale = 0.1;
		}
	}
	read.UnLoad();

	

	// �ŏ�ʂɓ�����p�[�c�̐ݒ�
	m_pTopGear = mGetProperty()._penemy->m_pBody;

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	mGetCharaEntity().mCreateRelationship(m_pTopGear, mGetProperty()._penemy->m_pWaist);
	mGetProperty()._colliderPosition = &m_pTopGear->_pGear->property._transform._translation;
	return true;

}

//�R���C�_�[�̏�����
void EnemyGround::mInitializeEnemyColider(ViewCamera* camera){
	mGetProperty()._pcolider = std::make_shared<Cube>();
	mGetProperty()._pcolider->Initialize();
	mGetProperty()._pcolider->property._transform._translation = m_pTopGear->_pGear->property._transform._translation;
	mGetProperty()._pcolider->property._transform._scale = 0.5;
	mGetProperty()._pcolider->property._color = Color(1, 1, 1, 0.5);
	mGetProperty()._pcolider->SetCamera(camera);
}

//�X�V����
void EnemyGround::mUpdate(){

	m_AI=GetAI();

	//AI��Update�̌Ăяo��
	m_AI->mUpdateRun(&mGetProperty(), &m_pTopGear->_pGear->property._transform._translation);

	mGetProperty()._pcolider->property._transform._translation = *mGetProperty()._colliderPosition;
}

void EnemyGround::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	if (mGetProperty().m_isRender)return;

	mGetProperty()._pcolider->Render(colider_shader);

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	mGetCharaEntity().mGearRender(m_pTopGear, model_shader, colider_shader);
}

void EnemyGround::SetLoadModelValue(std::shared_ptr<Gear>& gear, ObjectInfo* info){

	gear->_pGear->property._transform = info->_transform;
	gear->_initialTransform = info->_transform;
	if (gear->_pParent)
	{
		std::shared_ptr<Gear> pParent = gear->_pParent;
		// �ŏ�ʂƂ̍�
		gear->_topDifference._translation = gear->_pGear->property._transform._translation - m_pTopGear->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - m_pTopGear->_pGear->property._transform._rotation;

		// �e�Ƃ̍�
		gear->_parentDifference._translation = gear->_pGear->property._transform._translation - pParent->_pGear->property._transform._translation;
		gear->_parentDifference._rotation = gear->_pGear->property._transform._rotation - pParent->_pGear->property._transform._rotation;
	}

	return;
}



//EnemyAI
std::shared_ptr<EnemyAI> EnemyGround::GetAI(){

	switch (mGetCharaStatus()._nowAction)
	{
		//�ҋ@
	case eActionType::eWait:
		return std::make_shared<EnemyWait>();
		//�ړ�
	case eActionType::eMove:
		return std::make_shared<EnemyMove>();
	case eActionType::eNull:
		
	case eActionType::eAttack:
		//���S
	case eActionType::eDie:
		return std::make_shared<EnemyDie>();
		
	default:
		break;
	}

}

void EnemyGround::mFinalize(){
	if (mGetProperty()._penemy)
	{
		mGetProperty()._penemy->Release();
		mGetProperty()._penemy.reset();
		mGetProperty()._penemy= nullptr;
	}

	if (mGetProperty()._pcolider){
		mGetProperty()._pcolider->Finalize();
		mGetProperty()._pcolider.reset();
		mGetProperty()._pcolider = nullptr;
	}
	if (m_pTopGear)
	{
		m_pTopGear->Release();
		m_pTopGear.reset();
		m_pTopGear = nullptr;
	}

}

void EnemyGround::mGetSetEnemyAction(){
	
}

