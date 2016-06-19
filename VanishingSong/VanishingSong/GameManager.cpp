#include "GameManager.h"
#include "Const.h"
#include <GameController.h>
#include <WorldReader.h>
using namespace aetherClass;
GameManager::GameManager()
{
	WorldReader read;
	read.Load("data\\Field\\player_init.aether");
	for (auto& index : read.GetInputWorldInfo()._object){
		if (index->_name == "player_init"){

			m_prevPlayerTransform._translation = index->_transform._translation;
			m_prevPlayerTransform._translation._y = 0;
			m_prevPlayerTransform._rotation = index->_transform._rotation;
			m_prevPlayerTransform._scale._x = -1;
		}
	}
	read.UnLoad();
	m_bossState = eBossState::eUnVisible;
	m_fieldState = eFieldState::eTutorial;
	m_canStageNumber = NULL;
}


GameManager::~GameManager()
{
	m_prevPlayerTransform._translation = kVector3Zero;
	m_bossState = eBossState::eUnVisible;
	m_fieldState = eFieldState::eNull;
	m_canStageNumber = NULL;
}

//
void GameManager::mPushUsePlayer(eMusical type){
	if (m_players.find(type) != m_players.end() || type == eMusical::eNull)return;
	m_players.insert(std::make_pair(type, type));
}
GameManager& GameManager::mGetInstance(){
	static GameManager instance;
	return instance;
}
//
std::unordered_map<eMusical, eMusical>& GameManager::mGetUsePlayer(){
	return m_players;
}

//
Transform GameManager::mGetPlayerTransform(){
	return m_prevPlayerTransform;
}

//
void GameManager::mSetPlayerTransform(aetherClass::Transform trans){
	m_prevPlayerTransform = trans;
}

void GameManager::mBattleDataFile(std::string file){
	m_battleDataFile = file;
}


std::string GameManager::mBattleDataFile(){
	return m_battleDataFile;
}

//
void GameManager::mBossState(GameManager::eBossState state){
	m_bossState = state;
}

//
GameManager::eBossState GameManager::mBossState(){
	return m_bossState;
}

//
void GameManager::mFieldState(GameManager::eFieldState state){
	m_fieldState = state;
}

//
GameManager::eFieldState GameManager::mFieldState(){
	return m_fieldState;
}

//
void GameManager::mNote(eMusical type){
	for (auto index : m_noteArray){
		if (index == type)return;
	}
	m_noteArray.push_back(type);
}

//
std::vector<eMusical>& GameManager::mNote(){
	return m_noteArray;
}

//
void GameManager::mGetCanStage(const int stage){
	m_canStageNumber = stage;
}

//
int  GameManager::mGetCanStage()const{
	return m_canStageNumber;
}

FadeManager& GameManager::mfadeManager(){
	return m_fadeManager;
}

std::pair<int, Transform>&  GameManager::mPrevEnemy(){
	return m_prevEnemy;
}
void GameManager::mPrevEnemy(const int number, Transform tras){
	m_prevEnemy.first = number;
	m_prevEnemy.second = tras;

}
