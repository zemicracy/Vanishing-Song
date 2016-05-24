#include "BattlePlayerManager.h"

using namespace aetherClass;
BattlePlayerManager::BattlePlayerManager()
{
}


BattlePlayerManager::~BattlePlayerManager()
{
}

//
void BattlePlayerManager::mSetPlayer(eMusical type, Vector3 position, std::shared_ptr<GearFrame> gearframe){
	if (m_pBattlePlayers.find(type) != m_pBattlePlayers.end() || type == eMusical::eNull)return;
	m_pBattlePlayers[type].mInitialize(type, position, gearframe);
}

//
void BattlePlayerManager::mRender(aetherClass::ShaderBase* shader){
	for (auto& player : m_pBattlePlayers){
		player.second.mRender(shader);
	}
}

//
void BattlePlayerManager::mUpdate(const float update, eMusical){
	for (auto& player : m_pBattlePlayers){
		player.second.mUpdate(update);
	}
}