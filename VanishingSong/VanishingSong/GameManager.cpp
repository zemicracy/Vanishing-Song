#include "GameManager.h"
#include "Const.h"
#include <GameController.h>

using namespace aetherClass;
GameManager::GameManager()
{
	m_prevPlayerTransform._translation._y = 22.2f;
}


GameManager::~GameManager()
{
}

bool GameManager::mIsPause(){
	return m_isPause;
}

void GameManager::mPushUsePlayer(eMusical type){
	if (m_players.find(type) != m_players.end() || type == eMusical::eNull)return;
	m_players.insert(std::make_pair(type, type));
}


std::unordered_map<eMusical, eMusical>& GameManager::mGetUsePlayer(){
	return m_players;
}

aetherClass::Transform GameManager::mGetPlayerTransform(){
	return m_prevPlayerTransform;
}
void GameManager::mSetPlayerTransform(aetherClass::Transform trans){
	m_prevPlayerTransform = trans;
}