#include "BattleEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"

using namespace aetherClass;
BattleEnemy::BattleEnemy()
{
}

BattleEnemy::~BattleEnemy()
{
}

void BattleEnemy::mInitialize(eMusical type,eEnemyType enemytype, ViewCamera* camera,Vector3& pos){

	Finalize();

	m_isDie = false;
	
	m_enemy._gearFrame = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(type, enemytype);

	m_enemy._gearFrame = gearframe;

	// ÅãˆÊ‚É“–‚½‚éƒp[ƒc‚ÌÝ’è
	m_pTopGear = m_enemy._gearFrame->m_pBody;

	m_charaEntity.SetCamera(m_pTopGear, camera);

	m_enemy._gearFrame->m_pBody->_pGear->property._transform._scale = 1.5;

	m_charaEntity.mGearMove(m_pTopGear, pos, "+=");

	m_enemy._gearFrame->m_pBody->_pGear->property._transform._rotation._y -= 90;
}

BattleEnemy::Enemy& BattleEnemy::mGetEnemy(){

	return m_enemy;

}

void BattleEnemy::mUpdate(const float timescale){
	
}


void BattleEnemy::mOnAttack(){

}

void BattleEnemy::mOnDamage(){

}

void BattleEnemy::mRender(std::shared_ptr<ShaderBase> tex){

	if (m_isDie){
		return;
	}
	m_charaEntity.mGearRender(m_pTopGear, tex.get(), tex.get());
}

void BattleEnemy::misDie(){
	m_isDie = true;
}

//GearFrame‚Í‰ð•ú‚µ‚È‚¢
void BattleEnemy::Finalize(){
}