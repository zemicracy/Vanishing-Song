#include "GameManager.h"
#include "Const.h"
#include <GameController.h>

using namespace aetherClass;
GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::mInitialize(){
	m_day = kFirstDay;
	m_isPause = false;
	m_gameState = eGameState::eNull;
	m_gameMode = eGameMode::eNull;
	m_skillType = eSkillType::eNull;
	return;
}

//
void GameManager::mGameState(GameManager::eGameState state){
	m_gameState = state;
}

//
GameManager::eGameState GameManager::mGameState(){
	return m_gameState;
}

signed char GameManager::mGetToDay(){
	return m_day;
}


bool GameManager::mIsPause(){
	return m_isPause;
}


void GameManager::mNextDay(){
	/*	“ú‚ªãŒÀ‚È‚ç‰½‚à‚µ‚È‚¢		*/
	if (m_day > kLastDay)return;
	m_day += 1;

	return;
}

//
GameManager::eSkillType GameManager::mSkillType()const{
	return m_skillType;
}
//
void GameManager::mSkillType(const GameManager::eSkillType type){
	m_skillType = type;
}

//
GameManager::eGameMode GameManager::mGameMode()const{
	return m_gameMode;
}

//
void GameManager::mGameMode(const GameManager::eGameMode mode){
	m_gameMode = mode;
}