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
	
	m_enemy._model= std::make_shared<FbxModel>();

	m_enemy._model = ResourceManager::mGetInstance().mGetEnemyHash(type);
	m_enemy._model->SetCamera(camera);
	m_enemy._model->property._transform._translation = pos;
	m_enemy._model->property._transform._scale = 1.5;
	m_enemy._model->property._transform._rotation._y = -90;

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
	m_enemy._model->Render(tex.get());
}

void BattleEnemy::misDie(){
	m_isDie = true;
}

void BattleEnemy::Finalize(){
	if (m_enemy._model){
		m_enemy._model.reset();
	}
}