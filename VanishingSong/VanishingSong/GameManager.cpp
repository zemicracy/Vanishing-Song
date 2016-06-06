#include "GameManager.h"
#include "Const.h"
#include <GameController.h>

using namespace aetherClass;
GameManager::GameManager()
{
	m_prevPlayerTransform._translation._y = 22.2f;
	m_bossState = eBossState::eUnVisible;
	m_fieldState = eFieldState::eNull;
	m_canStageNumber = NULL;
}


GameManager::~GameManager()
{
	m_prevPlayerTransform._translation._y = 22.2f;
	m_bossState = eBossState::eUnVisible;
	m_fieldState = eFieldState::eNull;
	m_canStageNumber = NULL;
}

//
void GameManager::mPushUsePlayer(eMusical type){
	if (m_players.find(type) != m_players.end() || type == eMusical::eNull)return;
	m_players.insert(std::make_pair(type, type));
}

//
std::unordered_map<eMusical, eMusical>& GameManager::mGetUsePlayer(){
	return m_players;
}

//
aetherClass::Transform GameManager::mGetPlayerTransform(){
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