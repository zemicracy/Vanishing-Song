#include "EnemyGround.h"


using namespace aetherClass;

EnemyGround::EnemyGround()
{
}


EnemyGround::~EnemyGround()
{
}

bool EnemyGround::mSetUp(){
	return true;
}


bool EnemyGround::mInitialize(ViewCamera* camera){

	m_pGearFrame = std::make_shared<GearFrame>();

	// �̂̃p�[�c
	m_pGearFrame->m_pBody = GetCharaEntity().mSetUpGear("null", Gear::eType::eBody, camera);

	// ���̃p�[�c
	m_pGearFrame->m_pWaist = GetCharaEntity().mSetUpGear("null", Gear::eType::eWaist, camera);

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	GetCharaEntity().mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
		
	return true;

}

void EnemyGround::mUpdate(){

}

void EnemyGround::mRender(aetherClass::ShaderBase* colider_shader, aetherClass::ShaderBase* model_shader){
	if (!m_pGearFrame->m_pBody)return;

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	GetCharaEntity().mGearRender(m_pGearFrame->m_pBody, colider_shader, model_shader);

}
