#include "BattlePlayerManager.h"

using namespace aetherClass;
BattlePlayerManager::BattlePlayerManager()
{
	m_animationName.insert(std::make_pair(BattlePlayer::eBattleActionType::eWait, "attackwait"));
	m_animationName.insert(std::make_pair(BattlePlayer::eBattleActionType::eAttack, "attack"));
	m_animationName.insert(std::make_pair(BattlePlayer::eBattleActionType::eDamage, "damage"));
	m_animationName.insert(std::make_pair(BattlePlayer::eBattleActionType::eWin, "win"));
}


BattlePlayerManager::~BattlePlayerManager()
{
}

//
void BattlePlayerManager::mSetPlayer(eMusical type, Vector3 position, std::shared_ptr<FbxModel> model){
	if (m_pBattlePlayers.find(type) != m_pBattlePlayers.end() || type == eMusical::eNull)return;
	m_pBattlePlayers[type].mInitialize(type, position, model);

}

//
void BattlePlayerManager::mRender(aetherClass::ShaderBase* shader){
	for (auto& player : m_pBattlePlayers){
		player.second.mRender(shader);

	}
}

//
void BattlePlayerManager::mUpdate(const float update, eMusical type){
	for (auto& player : m_pBattlePlayers){
		player.second.mUpdate(update);
	}
}

void BattlePlayerManager::mChangeAnimation(BattlePlayer::eBattleActionType action, eMusical color){
	if (m_pBattlePlayers.find(color) == m_pBattlePlayers.end() && color != eMusical::eMiss)return;

	if (color == eMusical::eMiss){
		for (auto &itr : m_pBattlePlayers){
			itr.second.mChangeAnimation(m_animationName.at(action));
		}
	}
	else if (action == BattlePlayer::eBattleActionType::eAttack || action == BattlePlayer::eBattleActionType::eWin){
			m_pBattlePlayers.at(color).mChangeAnimation(m_animationName.at(action));
	}
}

