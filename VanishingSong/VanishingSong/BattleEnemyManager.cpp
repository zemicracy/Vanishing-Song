#include "BattleEnemyManager.h"
#include <random>
#include "Cipher.h"
using namespace aetherClass;
BattleEnemyManager::BattleEnemyManager()
{
}


BattleEnemyManager::~BattleEnemyManager()
{
}

void BattleEnemyManager::Initialize(ViewCamera* camera,BattleField* lane){
	
	m_BattleField = lane;

	auto transform = m_BattleField->mGetEnemyLane(eMusical::eBlue);
	m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<BattleEnemy>());
	m_pEnemy.begin()->get()->mInitialize(eMusical::eBlue, camera,transform);
	
	transform = m_BattleField->mGetEnemyLane(eMusical::eGreen);
	m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<BattleEnemy>());
	m_pEnemy.begin()->get()->mInitialize(eMusical::eGreen, camera, transform);


	mLoadInfo("Battle\\Stage1");

}


void BattleEnemyManager::mRender(std::shared_ptr<ShaderBase> tex){
	
	for (int i = 0; i < m_pEnemy.size(); i++){
		m_pEnemy[i]->mRender(tex);
	}
}


void BattleEnemyManager::AddList(){

	BlueAdd();
}

std::vector<eMusical>  BattleEnemyManager::GetList(){

	m_enemyList.clear();

	AddList();

	return m_enemyList;
}

void BattleEnemyManager::BlueAdd(){

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, 400);

	int r;
	int random = rand100(rnd);

	if (random < 100){
		r = 0;
	}else if (random > 100 && random <= 200){
		r = 1;
	}
	else if (random > 200 && random <= 300){
		r = 2;
	}
	else if (random > 300 && random <= 400){
		r = 3;
	}

	switch (r)
	{
	case 0: // › › ›››
		for (int i = 0; i <8; i++){
			m_enemyList.push_back(eMusical::eBlue);
		}
		m_enemyList.insert(m_enemyList.begin(), eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 2,eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 4, eMusical::eNull);
		break;
	case 1://› › › ››
		for (int i = 0; i < 8; i++){
			m_enemyList.push_back(eMusical::eBlue);
		}
		m_enemyList.insert(m_enemyList.begin() + 1, eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 3, eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 5, eMusical::eNull);
		break;
	case 2://  ›››› 
		for (int i = 0; i < 8; i++){
			m_enemyList.push_back(eMusical::eBlue);
		}
		m_enemyList.insert(m_enemyList.begin() + 1, eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 2, eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 6, eMusical::eNull);

		break;
	case 3:
		for(int i = 0; i < 8; i++){
			m_enemyList.push_back(eMusical::eBlue);
		}
		m_enemyList.insert(m_enemyList.begin() + 3, eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 4, eMusical::eNull);
		m_enemyList.insert(m_enemyList.begin() + 6, eMusical::eNull);
		break;
		
	default:
		break;
	}
}

void BattleEnemyManager::mUpadate(const float timeScale){
	
}

void BattleEnemyManager::mLoadInfo(std::string path){
	Cipher chipher(path);
	chipher.mConsoleFind();
	int rrr;
	/*for (auto splitString : chipher.mGetSpriteArray("[Wave1]")){
		if (splitString.front() == 'r'){

		}
	}*/
}
