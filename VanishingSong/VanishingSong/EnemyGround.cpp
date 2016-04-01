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

	// 体のパーツ
	m_pGearFrame->m_pBody = m_charaEntity.mSetUpGear("null", Gear::eType::eBody, camera);

	// 腰のパーツ
	m_pGearFrame->m_pWaist = m_charaEntity.mSetUpGear("null", Gear::eType::eWaist, camera);

	// 体にパーツとの親子関係
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
		
	return true;

}

void EnemyGround::mUpdate(){

}

void EnemyGround::mRender(aetherClass::ShaderBase* shader){
	if (!m_pGearFrame->m_pBody)return;

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_pGearFrame->m_pBody, shader);

}
