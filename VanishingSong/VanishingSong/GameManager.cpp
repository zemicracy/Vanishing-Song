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

	m_isPause = false;
	m_gameState = eGameState::eNull;
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

bool GameManager::mIsPause(){
	return m_isPause;
}