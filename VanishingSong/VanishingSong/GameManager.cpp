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

bool GameManager::mIsPause(){
	return m_isPause;
}

void GameManager::mPushUsePlayer(eMusical type){
	m_players.push_back(type);
}


std::vector<eMusical>& GameManager::mGetUsePlayer(){
	return m_players;
}