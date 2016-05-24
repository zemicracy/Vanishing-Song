#ifndef _BATTLEPLAYERMANAGER_H
#define _BATTLEPLAYERMANAGER_H
#include <unordered_map>
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
	void mSetPlayer(eMusical,aetherClass::Vector3,std::shared_ptr<GearFrame>);
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(const float,eMusical);
private:
	std::unordered_map<eMusical, BattlePlayer> m_pBattlePlayers;
};

#endif