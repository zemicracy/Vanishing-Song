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

void GameManager::Initialize(){
	m_day = kFirstDay;
	m_isPause = false;
	m_gameState = eState::eNull;
	return;
}

//
void GameManager::GameState(GameManager::eState state){
	m_gameState = state;
}

//
GameManager::eState GameManager::GameState(){
	return m_gameState;
}

signed char GameManager::GetToDay(){
	return m_day;
}


bool GameManager::IsPause(){
	return m_isPause;
}


void GameManager::NextDay(){
	/*	日が上限なら何もしない		*/
	if (m_day > kLastDay)return;
	m_day += 1;

	return;
}

// ポーズボタンが押されたかの判定
void GameManager::CheckPauseKey(){
	if (GameController::GetKey().KeyDownTrigger(VK_PAUSE))
	{
		m_isPause = !m_isPause;
		Debug::mPrint("PauseState :" + std::to_string(m_isPause));
	}
}