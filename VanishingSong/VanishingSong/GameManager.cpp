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
	/*	日が上限なら何もしない		*/
	if (m_day > kLastDay)return;
	m_day += 1;

	return;
}

// ポーズボタンが押されたかの判定
void GameManager::mCheckPauseKey(){
	if (GameController::GetKey().KeyDownTrigger(VK_PAUSE))
	{
		m_isPause = !m_isPause;
		Debug::mPrint("PauseState :" + std::to_string(m_isPause));
	}
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