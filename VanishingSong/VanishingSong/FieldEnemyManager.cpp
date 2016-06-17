#include "FieldEnemyManager.h"
#include "FieldEnemy.h"
#include <random>
#include "PixelShader.h"
#include <chrono>
#include "WorldReader.h"
#include "Singleton.h"
#include "ResourceManager.h"
#include"GameManager.h"

using namespace aetherClass;
FieldEnemyManager::FieldEnemyManager()
{
}

FieldEnemyManager::~FieldEnemyManager()
{
	mFinalize();
}

bool FieldEnemyManager::mInitilize(aetherClass::ViewCamera* camera){
	
	EnemySize = 1;

	m_bossFlag = GameManager::mGetInstance().mBossState() != GameManager::eBossState::eUnVisible ? true : false;

	WorldReader reader;
	reader.Load("data\\Field\\stage.aether");
	//Spawn場所
	for (auto index : reader.GetInputWorldInfo()._object){
		if (index->_name == "area1"){
			m_pEnemySpawner[0] = index->_transform._translation;
		}
		if (index->_name == "area2"){
			m_pEnemySpawner[1] = index->_transform._translation;
		}
		if (index->_name == "area3"){
			m_pEnemySpawner[2] = index->_transform._translation;
		}
		if (index->_name == "area4"){
			m_pEnemySpawner[3] = index->_transform._translation;
		}
		if (index->_name == "area5"){
			m_pEnemySpawner[4] = index->_transform._translation;
		}
	}
	reader.UnLoad();

	m_pEnemy.reserve(5);

	//EnemyGroundの生成
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.push_back(std::make_shared<FieldEnemy>());
		m_pEnemy.back()->mInitialize(eMusical::eBlue,camera,"data\\Battle\\Stage1");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mGetProperty()._penemy->property._transform._rotation._y = 180;

	}

	//EnemyAirの生成
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.push_back(std::make_shared<FieldEnemy>());
		m_pEnemy.back()->mInitialize(eMusical::eGreen, camera, "data\\Battle\\Stage2");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mGetProperty()._penemy->property._transform._rotation._y = 180;
	}

	//Enemy(仮)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.push_back(std::make_shared<FieldEnemy>());
		m_pEnemy.back()->mInitialize(eMusical::eRed, camera, "data\\Battle\\Stage3");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mGetProperty()._penemy->property._transform._rotation._y = 0;
	}

	//Enemy(仮)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.push_back(std::make_shared<FieldEnemy>());
		m_pEnemy.back()->mInitialize(eMusical::eYellow, camera, "data\\Battle\\Stage4");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mGetProperty()._penemy->property._transform._rotation._y = 0;
	}

	//Enemy(仮)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.push_back(std::make_shared<FieldEnemy>());
		m_pEnemy.back()->mInitialize(eMusical::eAdlib, camera, "data\\Battle\\Stage5");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
	}


	//敵の初期位置
	mSetPosion();

	m_isRender = false;
	m_isJudge = false;
	return true;
}


//描画処理
void FieldEnemyManager::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
		for (int i = 0; i < 4; i++){
			m_pEnemy[i]->mRender(model_shader, colider_shader);
		}

		if (m_bossFlag){
			m_pEnemy[4]->mRender(model_shader, colider_shader);
		}

		
}

bool FieldEnemyManager::mGetIsJudge(){

	return m_isJudge;
}

//更新処理
void FieldEnemyManager::mUpdater(){

	for (auto itr : m_pEnemy){
	itr->mUpdate();
	}

}


//解放処理(コライダーはしなくていい)
void FieldEnemyManager::mFinalize(){


	for (auto &itr : m_pEnemy){
		itr.reset();
	}
	for (auto &itr : m_enemyArray){
		if (itr){
			itr->mFinalize();
			itr.reset();
		}
	}	
}

void FieldEnemyManager::mSetPosion(){

	//敵の出現位置
	for (int i = 0; i <m_pEnemy.size(); i++){
		m_pEnemy[i]->mGetProperty()._penemy->property._transform._translation = m_pEnemySpawner[i];
		m_pEnemy[i]->mGetProperty()._penemy->property._transform._translation._y = 20;
		m_pEnemy[i]->mGetProperty()._enemyAreaNo = i;
		m_enemyArray[i]=m_pEnemy[i];
	}
}

std::shared_ptr<FieldEnemy> FieldEnemyManager::mEnemyGet(int enemy){
	return  m_enemyArray[enemy];
}