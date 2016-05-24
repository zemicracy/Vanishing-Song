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
	if (m_players.find(type) != m_players.end())return;
	m_players.insert(std::make_pair(type, type));
}


std::unordered_map<eMusical, eMusical>& GameManager::mGetUsePlayer(){
	return m_players;
}