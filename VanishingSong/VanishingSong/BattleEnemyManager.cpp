#include "BattleEnemyManager.h"
#include <random>
#include "Cipher.h"
#include"ResourceManager.h"
#include"OrderList.h"

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
	m_fieldState = GameManager::mGetInstance().mFieldState();
	m_optionCount = 0;

	m_animationName.insert(std::make_pair(BattleEnemy::eBattleActionType::eWait, "wait"));
	m_animationName.insert(std::make_pair(BattleEnemy::eBattleActionType::eAttack, "attack"));
	m_animationName.insert(std::make_pair(BattleEnemy::eBattleActionType::eDamage, "damage"));

}


void BattleEnemyManager::mRender(std::shared_ptr<ShaderBase> tex){
	for (auto &itr : m_pEnemy){
		itr.second->mRender(tex);
	}
}

std::vector<eMusical>  BattleEnemyManager::GetList(){

	m_enemyList.clear();

	int random = mGetRandom();

	std::vector<eMusical>vec;
	vec.reserve(m_enemyAttackList[random].size());

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, m_enemyAttackList[random].size());
	int adlib = rand100(rnd);

	for (int i = 0; i < m_enemyAttackList[random].size(); ++i){
		auto itr = m_enemyAttackList[random][i];
			if (i == adlib){
				std::uniform_int_distribution<> randDevice(0, 100);
				int makeAdlib = randDevice(rnd);
				if (makeAdlib >= 50) itr = eMusical::eAdlib;
		}
		vec.push_back(itr);
	}
	return vec;
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

void BattleEnemyManager::mUpdate(const float timeScale ){
	for (auto &itr : m_pEnemy){
		itr.second->mUpdate(1);
	}
	
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
			enemy.second.reset();
		}
		m_pEnemy.clear();
	}

	for (auto& enemyType : m_waveEnemyList[waveCount]){
		m_pEnemy.insert(std::make_pair(enemyType->mGetType(),enemyType));
	}

}

void BattleEnemyManager::misDie(){
	for (auto& enemy : m_pEnemy){
		enemy.second->misDie();
	}
}

int BattleEnemyManager::mGetAppendOption(){

	if (m_stageID == 1){
		if (m_waveID == 1){
			if (m_optionCount >= 2){
				m_optionCount = 0;
				return OrderList::eAppendOption::eBlack;
			}
			else{
				m_optionCount++;
			}
		}
	}
	else if (m_stageID == 2){
		if (m_waveID == 1){
			if (m_optionCount >= 1){
				m_optionCount = 0;
				return OrderList::eAppendOption::eBlack;
			}
			else{
				m_optionCount++;
			}	
		}
		else if (m_waveID == 2){
			return OrderList::eAppendOption::eBlack;
		}
	}
	else if (m_stageID == 3){
		if (m_waveID == 1){
			return OrderList::eAppendOption::eReverce;
		}
		else if (m_waveID == 2){
			if (m_optionCount >= 2){
				m_optionCount = 0;
				return OrderList::eAppendOption::eReverce;
			}
			else{
				m_optionCount++;
			}
		}
	}
	else if (m_stageID == 4){
			std::random_device rnd;
			std::mt19937 mt(rnd());
			std::uniform_int_distribution<> rand100(0, 100);
			int rand = rand100(rnd);
			if (m_waveID == 1){
				if (rand > 70){
					return OrderList::eAppendOption::eBlack;
				}
			}
		else if (m_waveID == 2){
			int returnOption = OrderList::eAppendOption::eReverce;
			if (rand > 65){
				returnOption |= OrderList::eAppendOption::eBlack;
			}
			return returnOption;
		}
	}
	else if (m_stageID == 5){
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> rand100(0, 100);
		int rand = rand100(rnd);

		if (m_waveID == 1){
			if (m_optionCount >= 1){
				m_optionCount = 0;
				return OrderList::eAppendOption::eBlack;
			}
			else{
				m_optionCount++;
			}
		}
		if (m_waveID == 2){
			if (0.25 > m_hp[m_waveID]._hp / m_hp[m_waveID]._maxHp){
				int returnOption = OrderList::eAppendOption::eBlack;
				if (rand > 50){
					returnOption |= OrderList::eAppendOption::eReverce;
				}
				return returnOption;
			}
			else if (0.50 > m_hp[m_waveID]._hp / m_hp[m_waveID]._maxHp){
				return OrderList::eAppendOption::eBlack;
			}
			else {
				if (rand > 75){
					return OrderList::eAppendOption::eBlack;
				}
				else if (rand > 40){
					return OrderList::eAppendOption::eReverce;
				}
			}
		}
	}
	return OrderList::eAppendOption::eNone;
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
		enemy.second.reset();
	}

	m_waveEnemyList.clear();
	m_enemyList.clear();
	m_enemyAttackList.clear();
	m_hp.clear();

}

void BattleEnemyManager::mChangeAnimation(BattleEnemy::eBattleActionType action, eMusical color){
	if (m_pEnemy.find(color) == m_pEnemy.end() && color != eMusical::eMiss)return;

	if (color == eMusical::eMiss){
		for (auto &itr : m_pEnemy){
			itr.second->mChangeAnimation(m_animationName.at(action));
		}
	}
	else if (action == BattleEnemy::eBattleActionType::eAttack){
		if (m_pEnemy.begin()->second->mGetType() == eMusical::eAdlib){
			m_pEnemy.at(eMusical::eAdlib)->mChangeAnimation(m_animationName.at(action));
		}
		else{
			m_pEnemy.at(color)->mChangeAnimation(m_animationName.at(action));
		}
	}
}

