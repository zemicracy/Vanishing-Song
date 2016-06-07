#include "BattleEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"

using namespace aetherClass;
BattleEnemy::BattleEnemy()
{
}

BattleEnemy::~BattleEnemy()
{
	Finalize();
}

void BattleEnemy::mInitialize(eMusical type,eEnemyType enemytype, ViewCamera* camera,Vector3& pos){

	m_isDie = false;
	
	m_enemy._gearFrame = std::make_shared<GearFrame>();

	switch (type)
	{
	case eMusical::eBlue:
		if (enemytype==eEnemyType::eAir){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Air\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Air\\Blue");
		}
		else if (enemytype == eEnemyType::eGround){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Ground\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Ground\\Blue");
		}
		break;
	case eMusical::eGreen:
		if (enemytype == eEnemyType::eAir){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Air\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Air\\Green");
		}
		else if (enemytype == eEnemyType::eGround){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Ground\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Ground\\Green");
		}
		break;
	case eMusical::eRed:
		if (enemytype == eEnemyType::eAir){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Air\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Air\\Red");
		}
		else if (enemytype == eEnemyType::eGround){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Ground\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Ground\\Red");
		}
		break;
	case eMusical::eYellow:
		if (enemytype == eEnemyType::eAir){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Air\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Air\\Yellow");
		}
		else if (enemytype == eEnemyType::eGround){
			m_enemy._gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Ground\\body.fbx", Gear::eType::eBody, "Model\\Enemy\\Ground\\Yellow");
		}
		break;
	default:
		break;
	}
	
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

void BattleEnemy::Finalize(){
	if (m_enemy._gearFrame){
		m_enemy._gearFrame->Release();
		m_enemy._gearFrame.reset();
	}
}