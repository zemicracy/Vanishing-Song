#include "FieldEnemyManager.h"
#include "FieldEnemy.h"
#include <random>
#include "PixelShader.h"
#include <chrono>
#include "WorldReader.h"

using namespace aetherClass;
FieldEnemyManager::FieldEnemyManager()
{
}

FieldEnemyManager::~FieldEnemyManager()
{
}

bool FieldEnemyManager::mInitilize(aetherClass::ViewCamera* camera){
	
	EnemyGroundSize = 2;
	EnemyAirSize = 2;

	EnemySize = EnemyGroundSize + EnemyAirSize;

	WorldReader reader;
	reader.Load("data\\EnemySpawnDay1.aether");

	//Spawn場所
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


	//EnemyGroundの生成
	for (int i = 0; i < EnemyGroundSize; i++){
		m_pEnemy.insert(m_pEnemy.begin(),std::make_shared<FieldEnemy>());
		m_pEnemy.begin()->get()->mInitializeGround(camera);
		m_pEnemy.begin()->get()->mInitializeEnemyColider(camera);
	}

	//EnemyGroundの生成
	for (int i = 0; i < EnemyAirSize; i++){
		m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<FieldEnemy>());
		m_pEnemy.begin()->get()->mInitializeAir(camera);
		m_pEnemy.begin()->get()->mInitializeEnemyColider(camera);
	}
	
	//敵の初期位置
	mSetPosion();
	
	return true;
}


//描画処理
void FieldEnemyManager::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
		for (int i = 0; i < m_pEnemy.size(); i++){
			m_pEnemy[i]->mRender(model_shader, colider_shader);
		}
}

//更新処理
void FieldEnemyManager::mUpdater(){

	for (auto itr : m_pEnemy){
	itr->mUpdate();
	}
}

//解放処理(コライダーはしなくていい)
void FieldEnemyManager::mFinalize(){
	
	for (auto itr:m_pEnemy){
	itr->mFinalize();
	}

}

void FieldEnemyManager::mSetPosion(){

	//敵の出現位置
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy[i]->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation = m_pEnemySpawner[i];
		m_pEnemy[i]->mGetProperty()._penemy->m_pWaist->_pGear->property._transform._translation = m_pEnemy[i]->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation;
		m_pEnemy[i]->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation._y = +20;
		m_pEnemy[i]->mGetProperty()._penemy->m_pWaist->_pGear->property._transform._translation._y = m_pEnemy[i]->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation._y - 2;
		m_pEnemy[i]->mGetProperty()._enemyAreaNo = i;
		m_enemyArray[i].push_back(m_pEnemy[i]);
	}

}

std::vector<std::shared_ptr<FieldEnemy>> FieldEnemyManager::mEnemyGet(int enemy){
	return  m_enemyArray[enemy];
}
