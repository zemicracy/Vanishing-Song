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

	m_enemy._model->SetColor(Color(0, 0, 0, 1));
}

BattleEnemy::Enemy& BattleEnemy::mGetEnemy(){
	return m_enemy;
}

void BattleEnemy::mUpdate(const float timescale){
	if (m_isDie){
		m_alpha -= 0.05;
		m_enemy._model->SetColor(Color(0, 0, 0, m_alpha));
	}
	else{
		m_enemy._model->SetColor(Color(0, 0, 0, 1));
	}
	
}


void BattleEnemy::mOnAttack(){

}

void BattleEnemy::mOnDamage(){

}

void BattleEnemy::mRender(std::shared_ptr<ShaderBase> tex){

	m_enemy._model->Render(tex.get());
}

void BattleEnemy::misDie(){
	m_isDie = true;
	m_alpha = 1;
}

void BattleEnemy::Finalize(){
	m_enemy._model->SetColor(Color(0, 0, 0, 1));
	if (m_enemy._model){
		m_enemy._model.reset();
	}
}