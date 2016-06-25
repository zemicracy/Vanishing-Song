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

	m_bossFlag = GameManager::mGetInstance().mBossState() != GameManager::eBossState::eUnVisible ? true : false;

	WorldReader reader;
	reader.Load("data\\Field\\stage",true);
	//SpawnêŠ
	for (auto index : reader.GetInputWorldInfo()._object){
		if (index->_name == "area1"){
			m_pEnemySpawner[0] = index->_transform._translation + Vector3(-120, 0, -150);
		}
		if (index->_name == "area2"){
			m_pEnemySpawner[1] = index->_transform._translation + Vector3(0, 0, -150);
		}
		if (index->_name == "area3"){
			m_pEnemySpawner[2] = index->_transform._translation;
		}
		if (index->_name == "area4"){
			m_pEnemySpawner[3] = index->_transform._translation+Vector3(100,0,50);
		}
		if (index->_name == "area5"){
			m_pEnemySpawner[4] = index->_transform._translation + Vector3(0, 0, 100);
		}
	}
	reader.UnLoad();
	EnemySize = 1;
	m_pEnemyList.reserve(5);

	//EnemyGround‚Ì¶¬
	for (int i = 0; i < EnemySize; i++){
		m_pEnemyList.push_back(std::make_shared<FieldEnemy>());
		m_pEnemyList.back()->mInitialize(eMusical::eBlue,camera,"data\\Battle\\Stage1");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");

		m_pEnemyList.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._rotation._y = 90;
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._scale._x = -1;
		m_pEnemyList.back()->mRegisterIcon("Texture\\Icon\\Enemy\\air2.png");

	}

	//EnemyAir‚Ì¶¬
	for (int i = 0; i < EnemySize; i++){
		m_pEnemyList.push_back(std::make_shared<FieldEnemy>());
		m_pEnemyList.back()->mInitialize(eMusical::eGreen, camera, "data\\Battle\\Stage2");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._rotation._y = 90;
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._scale._x = -1;
		m_pEnemyList.back()->mRegisterIcon("Texture\\Icon\\Enemy\\gro.png");
	}

	//Enemy(‰¼)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemyList.push_back(std::make_shared<FieldEnemy>());
		m_pEnemyList.back()->mInitialize(eMusical::eRed, camera, "data\\Battle\\Stage3");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._rotation._y = 90;
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._scale._x = -1;
		m_pEnemyList.back()->mRegisterIcon("Texture\\Icon\\Enemy\\danbaru.png");

	}

	//Enemy(‰¼)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemyList.push_back(std::make_shared<FieldEnemy>());
		m_pEnemyList.back()->mInitialize(eMusical::eYellow, camera, "data\\Battle\\Stage4");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._rotation._y = 0;
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._scale._x = -1;
		m_pEnemyList.back()->mRegisterIcon("Texture\\Icon\\Enemy\\annon.png");

	}

	//Enemy(‰¼)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemyList.push_back(std::make_shared<FieldEnemy>());
		m_pEnemyList.back()->mInitialize(eMusical::eAdlib, camera, "data\\Battle\\Stage5");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mRegisterCannnotMessage("Texture\\Message\\tmplate.png");
		m_pEnemyList.back()->mGetProperty()._pEnemy->property._transform._scale = Vector3( -1,1,1);
		m_pEnemyList.back()->mRegisterIcon("Texture\\Icon\\Enemy\\boss.png");

	}

	for (int i = 0; i < 4; i++){

		if (i < mFieldStateToInt(GameManager::mGetInstance().mFieldState())){
			if (m_pEnemyList[i]->mGetType() != eMusical::eYellow || m_pEnemyList[i]->mGetType() != eMusical::eRed){
				m_pEnemyList[i]->mGetProperty()._pEnemy->property._transform._rotation._y = 180;
			}
			else{
				m_pEnemyList[i]->mGetProperty()._pEnemy->property._transform._rotation._y = 0;
			}
		}
	}
	//“G‚Ì‰ŠúˆÊ’u
	mSetPosion();

	m_isRender = false;
	m_isJudge = false;
	return true;
}


//•`‰æˆ—
void FieldEnemyManager::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
		for (int i = 0; i < 4; i++){
			m_pEnemyList[i]->mRender(model_shader, colider_shader);
			m_pEnemyList[i]->mGetProperty()._pCollider->Render(colider_shader);
		}

		if (m_bossFlag){
			m_pEnemyList[4]->mRender(model_shader, colider_shader);
		}

		
}

bool FieldEnemyManager::mGetIsJudge(){

	return m_isJudge;
}

//XVˆ—
void FieldEnemyManager::mUpdater(){
	for (int i = 0; i < 4; i++){
		if (m_pEnemyList[i]->mGetType() != eMusical::eYellow){
			if (i > mFieldStateToInt(GameManager::mGetInstance().mFieldState())){
				m_pEnemyList[i]->mUpdate("attack");
			}
			else{
				m_pEnemyList[i]->mUpdate("wait");
			}
		}
		else{
			m_pEnemyList[i]->mUpdate("wait");
		}
	}

	if (m_bossFlag){
		m_pEnemyList[4]->mUpdate("wait");
	}

}

bool FieldEnemyManager::mGetBossFlg(){
	return m_bossFlag;
}


//‰ğ•úˆ—
void FieldEnemyManager::mFinalize(){
	for (auto &itr : m_pEnemyList){
		itr->mFinalize();
		itr.reset();
	}
}

//
void FieldEnemyManager::mSetPosion(){

	//“G‚ÌoŒ»ˆÊ’u
	for (int i = 0; i <m_pEnemyList.size(); i++){
		m_pEnemyList[i]->mGetProperty()._pEnemy->property._transform._translation = m_pEnemySpawner[i];
	
		m_pEnemyList[i]->mGetProperty()._enemyAreaNo = i;
		m_pEnemyList[i]->mSetTransform(m_pEnemyList[i]->mGetProperty()._pEnemy->property._transform);
	}
}

//
std::shared_ptr<FieldEnemy>& FieldEnemyManager::mEnemyGet(int enemy){
	return  m_pEnemyList[enemy];
}

//
void FieldEnemyManager::mResetEnemysTransform(){
	for (auto &itr : m_pEnemyList){
		itr->mResetTransform();
	}
}

int FieldEnemyManager::mFieldStateToInt(GameManager::eFieldState state){
	switch (state)
	{
	case GameManager::eFieldState::eFirstStage:
		return -1;
	case GameManager::eFieldState::eSecoundStage:
		return 0;
	case GameManager::eFieldState::eThirdStage:
		return 1;
	case GameManager::eFieldState::eForthStage:
		return 2;

	default:
		break;
	}

	return -50;
}
