#include "EnemyGround.h"


using namespace aetherClass;

EnemyGround::EnemyGround()
{
}


EnemyGround::~EnemyGround()
{
}


bool EnemyGround::mInitialize(ViewCamera* camera){
	m_pGearFrame = std::make_shared<GearFrame>();

	// �̂̃p�[�c
	m_pGearFrame->m_pBody = m_charaEntity.mSetUpGear("null", Gear::eType::eBody, camera);

	// ���̃p�[�c
	m_pGearFrame->m_pWaist = m_charaEntity.mSetUpGear("null", Gear::eType::eWaist, camera);

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
		
	return true;

}

void EnemyGround::mUpdate(){

}

void EnemyGround::mRender(aetherClass::ShaderBase* shader){
	if (!m_pGearFrame->m_pBody)return;

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	m_charaEntity.mGearRender(m_pGearFrame->m_pBody, shader);

}
