#include "BattleEnemyManager.h"
#include <random>
#include "Cipher.h"
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
	
	mLoadInfo("data\\Battle\\Stage1",lane,camera);
//	ResetEnemyList(0, camera);
	m_camera = camera;

	flag = true;


}


void BattleEnemyManager::mRender(std::shared_ptr<ShaderBase> tex){
	
	for (int i = 0; i < m_pEnemy.size(); i++){
		m_pEnemy[i]->mRender(tex);
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
	std::uniform_int_distribution<> rand100(0, 500);

	int r;
	int random = rand100(rnd);

	if (random <= 100){
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
	else if (random > 400 && random <= 500){
		r = 4;
	}
	/*}
	else if (random > 500 && random <= 600){
		r = 5;
	}
	else if (random > 600 && random <= 700){
		r = 6;
	}
	else if (random > 700 && random <= 800){
		r = 7;
	}
	else if (random > 800 && random <= 900){
		r = 8;
	}
	else if (random > 900 && random <= 1000){
		r = 9;
	}
	else if (random > 1000 && random <= 1100){
		r = 10;
	}*/


	if (r > m_attackAllCount-1){
		r = mGetRandom();
	}
	return r;
}

void BattleEnemyManager::mUpadate(const float timeScale ){
	
}

void BattleEnemyManager::mLoadInfo(std::string path,BattleField* lane ,ViewCamera* camera){
	Cipher chipher(path);
	chipher.mConsoleFind();

	m_waveAllCount =std::atoi(&chipher.mGetSpriteArray("[WaveAll]").front().front());
		
	m_BattleField = lane;

	for (int i = 0; i < m_waveAllCount; ++i){

		std::vector<std::pair<eMusical, eEnemyType>> waveEnemyList;

		for (auto splitString : chipher.mGetSpriteArray("[Wave"+std::to_string(i+1)+"]")){

			auto color = mGetEnemyColor(splitString.front());
			auto type = mGetEnemyType(splitString.back());

			waveEnemyList.push_back(std::make_pair(color,type));
		}
		m_waveEnemyList.push_back(waveEnemyList);
	}

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
}

CharaStatus& BattleEnemyManager::mGetCharaStatus(int index){
	return m_hp[index];

}

void BattleEnemyManager::ResetEnemyList(int waveCount,ViewCamera* camera){

	if (!m_pEnemy.empty()){
		for (auto& enemy : m_pEnemy){
			enemy.reset();
		}
		m_pEnemy.clear();
	}

	for (auto& enemyType : m_waveEnemyList[waveCount]){
		auto transform = m_BattleField->mGetEnemyLane(enemyType.first);
		m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<BattleEnemy>());
		m_pEnemy.begin()->get()->mInitialize(enemyType.first, enemyType.second, camera, transform);
	}

}

void BattleEnemyManager::misDie(){
	for (auto& enemy : m_pEnemy){
		enemy->misDie();
	}
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
		return eMusical::eNull;
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

	for (auto enemy :m_waveEnemyList){
		enemy.clear();
	}
	for (auto enemy : m_enemyAttackList){
		enemy.clear();
	}

}