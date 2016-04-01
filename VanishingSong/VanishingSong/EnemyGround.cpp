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

	// 体のパーツ
	m_pGearFrame->m_pBody = GetCharaEntity().mSetUpGear("null", Gear::eType::eBody, camera);

	// 腰のパーツ
	m_pGearFrame->m_pWaist = GetCharaEntity().mSetUpGear("null", Gear::eType::eWaist, camera);

	// 体にパーツとの親子関係
	GetCharaEntity().mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
		
	return true;

}

void EnemyGround::mUpdate(){

}

void EnemyGround::mRender(aetherClass::ShaderBase* colider_shader, aetherClass::ShaderBase* model_shader){
	if (!m_pGearFrame->m_pBody)return;

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	GetCharaEntity().mGearRender(m_pGearFrame->m_pBody, colider_shader, model_shader);

}
