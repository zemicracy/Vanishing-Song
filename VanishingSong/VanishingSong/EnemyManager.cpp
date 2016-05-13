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
	reader.Load("data\\EnemySpawnDay1.aether");

	//SpawnêŠ
	for (auto index : reader.GetInputWorldInfo()._object){

		if (index->_name == "area1Spawn"){
			m_pEnemySpawner[0] = index->_transform._translation;
		}
		if (index->_name == "area2Spawn"){
			m_pEnemySpawner[1] = index->_transform._translation;
		}
		if (index->_name == "area3Spawn"){
			m_pEnemySpawner[2] = index->_transform._translation;
		}
		if (index->_name == "area4Spawn"){
			m_pEnemySpawner[3] = index->_transform._translation;
		}
	}

	reader.UnLoad();

	WorldReader reader2;
	reader2.Load("data/stage.aether");

	//SpawnêŠ
	for (auto index : reader2.GetInputWorldInfo()._object){
		if (index->_name == "area1"){
			areaProperty[0].area_max_x = index->_transform._translation._x + index->_transform._scale._x;
			areaProperty[0].area_min_x = index->_transform._translation._x - index->_transform._scale._x;
			areaProperty[0].area_max_z = index->_transform._translation._z + index->_transform._scale._z;
			areaProperty[0].area_min_z = index->_transform._translation._z - index->_transform._scale._z;
		}
		if (index->_name == "area2"){
			areaProperty[1].area_max_x = index->_transform._translation._x + index->_transform._scale._x;
			areaProperty[1].area_min_x = index->_transform._translation._x - index->_transform._scale._x;
			areaProperty[1].area_max_z = index->_transform._translation._z + index->_transform._scale._z;
			areaProperty[1].area_min_z = index->_transform._translation._z - index->_transform._scale._z;
		}
		if (index->_name == "area3"){
			areaProperty[2].area_max_x = index->_transform._translation._x + index->_transform._scale._x;
			areaProperty[2].area_min_x = index->_transform._translation._x - index->_transform._scale._x;
			areaProperty[2].area_max_z = index->_transform._translation._z + index->_transform._scale._z;
			areaProperty[2].area_min_z = index->_transform._translation._z - index->_transform._scale._z;
		}
		if (index->_name == "area4"){
			areaProperty[3].area_max_x = index->_transform._translation._x + index->_transform._scale._x;
			areaProperty[3].area_min_x = index->_transform._translation._x - index->_transform._scale._x;
			areaProperty[3].area_max_z = index->_transform._translation._z + index->_transform._scale._z;
			areaProperty[3].area_min_z = index->_transform._translation._z - index->_transform._scale._z;
		}
	}

	reader2.UnLoad();

	//EnemyGround‚Ì¶¬
	for (int i = 0; i < 4; i++){
		m_pEnemy.insert(m_pEnemy.begin(),std::make_shared<EnemyGround>());
		m_pEnemy.begin()->get()->mInitialize(camera);
		m_pEnemy.begin()->get()->mInitializeEnemyColider(camera);
		m_pEnemy.begin()->get()->mGetProperty().m_isRender = true;
		m_pEnemy.begin()->get()->mGetProperty()._moveFlag = false;
		m_pEnemy.begin()->get()->mGetProperty()._onHitFlag = false;
	}
	
	//“G‚ÌÅ‘å”
	m_Enemy_Max = 0;
	time = 0;
	actiontime = 0;
	
	//“G‚ÌStatus‚Ì‰Šú‰»
	mStatusInit();
	//“G‚Ì‰ŠúˆÊ’u
	mSetPosion();
	
	return true;
}


void EnemyManager::mStatusInit(){

	for (auto itr:m_pEnemy){
		itr->mGetEnemyStatus()._hp = 1;
		itr->mGetEnemyStatus()._level = 1;
	}
}

void EnemyManager::mStatusUpdater(){

}

//•`‰æˆ—
void EnemyManager::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){

	for (int i = 0; i < m_pEnemy.size(); i++){
		m_pEnemy[i]->mRender(model_shader, colider_shader);
	}
}

//XVˆ—
void EnemyManager::mUpdater(){

	mChangeAction();
	mSpawn();

	for (auto itr : m_pEnemy){
	itr->mUpdate();
	}
}

//‰ğ•úˆ—(ƒRƒ‰ƒCƒ_[‚Í‚µ‚È‚­‚Ä‚¢‚¢)
void EnemyManager::mFinalize(){
	
	for (auto itr:m_pEnemy){
	itr->mFinalize();
	}

}

void EnemyManager::mSetPosion(){

	std::random_device randam;
	std::mt19937 mt(randam());
	std::uniform_int_distribution<> create(0, 3);

	//“G‚ÌoŒ»ˆÊ’u
	for (auto itr : m_pEnemy){
		int randomValue = create(randam);	
		itr->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation = m_pEnemySpawner[randomValue];
		itr->mGetProperty()._penemy->m_pWaist->_pGear->property._transform._translation = itr->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation;
		itr->mGetProperty()._penemy->m_pWaist->_pGear->property._transform._translation._y = itr->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation._y - 10;
		itr->mGetProperty()._enemyAreaNo = randomValue;
		itr->mGetProperty()._enemyMoveRange.enemy_max_x = areaProperty[randomValue].area_max_x;
		itr->mGetProperty()._enemyMoveRange.enemy_max_z = areaProperty[randomValue].area_max_z;
		itr->mGetProperty()._enemyMoveRange.enemy_min_x = areaProperty[randomValue].area_min_x;
		itr->mGetProperty()._enemyMoveRange.enemy_min_z = areaProperty[randomValue].area_min_z;
		m_enemyArray[randomValue].push_back(itr);
	}
}


//“G‚ÌoŒ»
void EnemyManager::mSpawn(){

	//•`‰æ
	time += GameClock::GetDeltaTime();
	if (m_Enemy_Max < 4){
		if (time > 2){
			m_pEnemy[m_Enemy_Max]->mGetProperty().m_isRender = false;
			time = 0;
			m_Enemy_Max++;
		}
	}
}

std::vector<std::shared_ptr<EnemyGround>> EnemyManager::mEnemyGet(int enemy){
	return  m_enemyArray[enemy];
}

//“G‚Ìs“®Ø‘Ö•”•ª
void EnemyManager::mChangeAction(){

}