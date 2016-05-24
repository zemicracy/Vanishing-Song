#include "BattleEnemyManager.h"

using namespace aetherClass;
BattleEnemyManager::BattleEnemyManager()
{
}


BattleEnemyManager::~BattleEnemyManager()
{
}

void BattleEnemyManager::Initialize(ViewCamera* camera,BattleField* lane){
	
	m_BattleField = lane;

	auto transform =m_BattleField->mGetLane(eMusical::eBlue)->property._transform;
	transform._translation._x += transform._scale._x;
	m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<BattleEnemy>());
	m_pEnemy.begin()->get()->mInitialize(eMusical::eBlue, camera,transform._translation);
	

	transform = m_BattleField->mGetLane(eMusical::eGreen)->property._transform;
	transform._translation._x += transform._scale._x;
	m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<BattleEnemy>());
	m_pEnemy.begin()->get()->mInitialize(eMusical::eGreen, camera, transform._translation);

}


void BattleEnemyManager::mRender(std::shared_ptr<ShaderBase> tex){
	
	for (int i = 0; i < m_pEnemy.size(); i++){
		m_pEnemy[i]->mRender(tex);
	}
}

void BattleEnemyManager::EnemySet(eMusical type, std::shared_ptr<GearFrame> gearFrame){
	

}

void BattleEnemyManager::mUpadate(const float timeScale){

}

