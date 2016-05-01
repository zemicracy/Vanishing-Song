#include "EnemyManager.h"
#include "EnemyMove.h"
#include "EnemyDie.h"
#include "EnemyWait.h"
#include "EnemyGround.h"
#include<random>
#include"PixelShader.h"
#include <chrono>
using namespace aetherClass;
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

bool EnemyManager::mInitilize(aetherClass::ViewCamera* camera){
	
	WorldReader reader;
	reader.Load("data\\EnemySpawner.aether");

	for (auto index : reader.GetInputWorldInfo()._object){

		if (index->_name == "first"){
			m_pEnemySpawner[0] = index->_transform._translation;
		}
		if (index->_name == "second"){
			m_pEnemySpawner[1] = index->_transform._translation;
		}
		if (index->_name == "third"){
			m_pEnemySpawner[2] = index->_transform._translation;
		}
		if (index->_name == "fouth"){
			m_pEnemySpawner[3] = index->_transform._translation;
		}
	}

	reader.UnLoad();

	m_pEnemy.resize(10);	

	for (int i = 0; i < 10; i++){
		m_pEnemy[i] = std::make_shared<EnemyGround>();
		m_pEnemy[i]->mInitialize(camera);
		m_pEnemy[i]->GetProperty().m_isRender = true;
	}

	
	
	
	mSetPosion();
	
	return true;
}

void EnemyManager::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){

	for (int i = 0; i < 4; i++){
	m_pEnemy[i]->mRender(model_shader,colider_shader);
	
	}
}

void EnemyManager::mUpdater(){

	mChangeAction();
	mSpawn();

	for (int i = 0; i < 4; i++){
		m_pEnemy[i]->mUpdate();
	}
}

void EnemyManager::mFinalize(){
	
	for (int i = 0; i < 4; i++){
		m_pEnemy[i]->mFinalize();
	}


}

void EnemyManager::mSetPosion(){

	std::random_device randam;
	std::mt19937 mt(randam());
	std::uniform_int_distribution<> create(0, 3);

	for (int i = 0; i < 4; i++){
		int randomValue = create(randam);
		m_pEnemy[i]->GetProperty()._enemy->m_pBody->_pGear->property._transform._translation = m_pEnemySpawner[randomValue];
		Debug::mPrint(std::to_string(randomValue), Debug::eState::eWindow);
	}

}


void EnemyManager::mSpawn(){

	static int time=0;
	static int i = 0;

	if (i < 10){
		if (time > 200){
			m_pEnemy[i]->GetProperty().m_isRender = false;
			time = 0;
			i++;
		}
	}
	time++;

}

void EnemyManager::mChangeAction(){
	if (GameController::GetKey().IsKeyDown('R')){
		m_pEnemy[0]->GetCharaStatus()._nowAction = eActionType::eDie;
	}
	if (GameController::GetKey().IsKeyDown('W')){
		m_pEnemy[0]->GetCharaStatus()._nowAction = eActionType::eMove;
		}
	
	if (GameController::GetKey().IsKeyDown('Q')){
		m_pEnemy[0]->GetCharaStatus()._nowAction = eActionType::eWait;
	}

}

