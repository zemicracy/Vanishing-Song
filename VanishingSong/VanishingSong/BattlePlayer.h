#ifndef _BATTLEPLAYER_H
#define _BATTLEPLAYER_H
#include "ResourceManager.h"
class BattlePlayer
{
public:
	BattlePlayer(ResourceManager::eMusical);
	~BattlePlayer();
	
private:
	void Initialize(ResourceManager::eMusical);
	

};

#endif