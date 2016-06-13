#include "BattleEnemyManager.h"
#include <random>
#include "Cipher.h"
#include"ResourceManager.h"
using namespace aetherClass;
int cnt = 0;

BattleEnemyManager::BattleEnemyManager()
{
}


BattleEnemyManager::~BattleEnemyManager()
{
	mFinalize();
}

void BattleEnemyManager::mInitialize(ViewCamera* camera,BattleField* lane){
	
	m_camera = camera;

	mLoadInfo(GameManager::mGetInstance().mBattleDataFile(),lane,camera);
	
	flag = true;

	
}


void BattleEnemyManager::mRender(std::shared_ptr<ShaderBase> tex){
	for (auto itr : m_pEnemy){
		itr->mRender(tex);
	}
}

std::vector<eMusical>  BattleEnemyManager::GetList(){

	m_enemyList.clear();

	int random = mGetRandom();

	return m_enemyAttackList[random];
}

int BattleEnemyManager::mGetRandom(){

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0,m_attackAllCount*100);

	int r;
	float random = rand100(rnd);

	r = int(random / 100);

	if (r > m_attackAllCount-1){
		r = mGetRandom();
	}
	return r;
}

void BattleEnemyManager::mUpadate(const float timeScale ){

}

void BattleEnemyManager::mLoadInfo(std::string path,BattleField* lane ,ViewCamera* camera){
	Cipher chipher(path);

	m_waveAllCount =std::atoi(&chipher.mGetSpriteArray("[WaveAll]").front().front());
		
	m_BattleField = lane;

	for (int i = 0; i < m_waveAllCount; ++i){

		std::vector<std::shared_ptr<BattleEnemy>> waveEnemyList;

		for (auto splitString : chipher.mGetSpriteArray("[Wave" + std::to_string(i + 1) + "]")){

			auto color = mGetEnemyColor(splitString.front());
			auto type = mGetEnemyType(splitString.back());

			std::shared_ptr<BattleEnemy> waveEnemy;
			waveEnemy = std::make_shared<BattleEnemy>();
			auto translation = m_BattleField->mGetEnemyLane(color);
			waveEnemy->mInitialize(color, type, m_camera, translation);
			waveEnemyList.push_back(waveEnemy);
		}
		m_waveEnemyList.push_back(waveEnemyList);	
	}

	m_stageID = std::atoi(&chipher.mGetSpriteArray("[Stage]").front().front());

	m_attackAllCount = std::atoi(&chipher.mGetSpriteArray("[AttackAll]").front().front());

	for (int i = 0; i < m_attackAllCount; ++i){

		std::vector<eMusical> attackList;

		for (auto splitString : chipher.mGetSpriteArray("[Attack" + std::to_string(i + 1) + "]")){

			auto attack = mGetEnemyAttack(splitString.front());
			attackList.push_back(attack);
		}
		m_enemyAttackList.push_back(attackList);
	}

	for (int i = 0; i < m_waveAllCount; ++i){
		for (auto splitString : chipher.mGetSpriteArray("[HP" + std::to_string(i + 1) + "]")){
			CharaStatus hp;
			hp._hp = hp._maxHp = std::atoi(splitString.c_str());
			m_hp.push_back(hp);
		}
	}

	chipher.mUnLoad();
}

CharaStatus& BattleEnemyManager::mGetCharaStatus(int index){

	return m_hp[index];

}

void BattleEnemyManager::ResetEnemyList(int waveCount,ViewCamera* camera){

	m_waveID = waveCount;

	if (!m_pEnemy.empty()){
		for (auto& enemy : m_pEnemy){
			enemy.reset();
		}
		m_pEnemy.clear();
	}

	for (auto& enemyType : m_waveEnemyList[waveCount]){
		m_pEnemy.push_back(enemyType);
	}

}

void BattleEnemyManager::misDie(){
	for (auto& enemy : m_pEnemy){
		enemy->misDie();
	}
}

int BattleEnemyManager::mGetAppendOption(){

	if (m_stageID < 3){
		return eAppendOption::eNone;
	}
	if (m_stageID == 4){
		if (m_waveID == 2){
			return eAppendOption::eReverce;
		}
	}

	if (m_stageID == 5){
		if (m_waveID == 2){
			return eAppendOption::eBlack;
		}
	}
	
	return eAppendOption::eNone;
}


eMusical BattleEnemyManager::mGetEnemyColor(const char colorChar){

	if (colorChar == 'b'){
		return eMusical::eBlue;
	}
	else if (colorChar == 'g'){
		return eMusical::eGreen;
	}
	else if (colorChar == 'r'){
		return eMusical::eRed;
	}
	else if (colorChar == 'y'){
		return eMusical::eYellow;
	}
	else if (colorChar == 'a'){
		return eMusical::eAdlib;
	}
	
	return eMusical::eNull;
}

eEnemyType  BattleEnemyManager::mGetEnemyType(const char typeChar){

	if (typeChar == '1'){
		return eEnemyType::eGround;
	}
	else if(typeChar == '2'){
		return eEnemyType::eAir;
	}
	else if (typeChar == '3'){
		return eEnemyType::eBoss;
	}
	return eEnemyType::eNull;
}

int BattleEnemyManager::mGetWaveAllCount(){

	return m_waveAllCount;
}

eMusical BattleEnemyManager::mGetEnemyAttack(const char attack){

	if (attack == 'b'){
		return eMusical::eBlue;
	}
	else if (attack == 'g'){
		return eMusical::eGreen;
	}
	else if (attack == 'r'){
		return eMusical::eRed;
	}
	else if (attack == 'y'){
		return eMusical::eYellow;
	}
	else if (attack == 'k'){
		return eMusical::eNull;
	}
	else if (attack == 'a'){
		return eMusical::eAdlib;
	}
	else if (attack == 'n'){
		return eMusical::eNull;
	}

	return eMusical::eNull;
}

void BattleEnemyManager:: mFinalize(){
	
	for (auto enemy : m_pEnemy){
		enemy.reset();
	}

	m_waveEnemyList.clear();
	m_enemyList.clear();
	m_enemyAttackList.clear();
	m_hp.clear();

}