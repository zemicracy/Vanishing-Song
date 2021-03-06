#ifndef _BATTLEPLAYERMANAGER_H
#define _BATTLEPLAYERMANAGER_H
#include <unordered_map>
#include <FbxModel.h>
#include "BattlePlayer.h"
#include "Const.h"
namespace{
	const int kPlayerNum = 4;
}

class BattlePlayerManager
{
public:
	BattlePlayerManager();
	~BattlePlayerManager();
	void mSetPlayer(eMusical,aetherClass::Vector3,std::shared_ptr<aetherClass::FbxModel>);
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(const float,eMusical);
	void BattlePlayerManager::mChangeAnimation(BattlePlayer::eBattleActionType action, eMusical color);
private:
	std::unordered_map<eMusical, BattlePlayer> m_pBattlePlayers;
	std::unordered_map<BattlePlayer::eBattleActionType, std::string>m_animationName;

};

#endif