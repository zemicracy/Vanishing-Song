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

	//SpawnêŠ
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

	m_pEnemy.resize(15);	
	//EnemyGround‚Ì¶¬
	for (int i = 0; i < 10; i++){
		m_pEnemy[i] = std::make_shared<EnemyGround>();
		m_pEnemy[i]->mInitialize(camera);
		m_pEnemy[i]->mInitializeEnemyColider(camera);
		m_pEnemy[i]->mGetProperty().m_isRender = true;
	}
	
	//“G‚ÌÅ‘å”
	m_Enemy_Max = 0;
	time = 0;
	


	//“G‚ÌStatus‚Ì‰Šú‰»
	mStatusInit();
	//“G‚Ì‰ŠúˆÊ’u
	mSetPosion();
	//“G‚Ìs“®”ÍˆÍ
	mSetMap();

	return true;
}


void EnemyManager::mStatusInit(){

	for (int i = 0; i < 10; i++){
		m_pEnemy[i]->mGetEnemyStatus()._hp = 1;
		m_pEnemy[i]->mGetEnemyStatus()._level = 1;
	}
}

void EnemyManager::mStatusUpdater(){


}

//•`‰æˆ—
void EnemyManager::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){

	for (int i = 0; i < 4; i++){
	m_pEnemy[i]->mRender(model_shader,colider_shader);
	}

}

//XVˆ—
void EnemyManager::mUpdater(){

	mChangeAction();
	mSpawn();

	for (int i = 0; i < 4; i++){
		m_pEnemy[i]->mUpdate();
	}
}

//‰ğ•úˆ—(ƒRƒ‰ƒCƒ_[‚Í‚µ‚È‚­‚Ä‚¢‚¢)
void EnemyManager::mFinalize(){
	
	for (int i = 0; i < 4; i++){
		m_pEnemy[i]->mFinalize();
	}

}

void EnemyManager::mSetPosion(){

	std::random_device randam;
	std::mt19937 mt(randam());
	std::uniform_int_distribution<> create(0, 3);

	//“G‚ÌoŒ»ˆÊ’u
	for (int i = 0; i < 4; i++){
		int randomValue = create(randam);
		m_pEnemy[i]->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation = m_pEnemySpawner[randomValue];
	}
}


void EnemyManager::mSetMap(){

	m_EnemyMap["Northwest"] = m_pEnemy[0];	//–k¼
	m_EnemyMap["Northeast"] = m_pEnemy[1];	//–k“Œ
	m_EnemyMap["Southwest"] = m_pEnemy[2];	//“ì¼
	m_EnemyMap["Southeast"] = m_pEnemy[3];	//–k“Œ
}

//“G‚ÌoŒ»
void EnemyManager::mSpawn(){

	//•`‰æ
	time += GameClock::GetDeltaTime();
	if (m_Enemy_Max < 2){
		if (time > 15){
			m_pEnemy[m_Enemy_Max]->mGetProperty().m_isRender = false;
			time = 0;
			m_Enemy_Max++;
		}
	}
}


//“G‚Ìs“®Ø‘Ö•”•ª
void EnemyManager::mChangeAction(){
	if (GameController::GetKey().IsKeyDown('R')){
		m_pEnemy[0]->mGetCharaStatus()._nowAction = eActionType::eDie;
	}
	if (GameController::GetKey().IsKeyDown('W')){
		m_pEnemy[0]->mGetCharaStatus()._nowAction = eActionType::eMove;
		}
	if (GameController::GetKey().IsKeyDown('Q')){
		m_pEnemy[0]->mGetCharaStatus()._nowAction = eActionType::eWait;
	}

}

