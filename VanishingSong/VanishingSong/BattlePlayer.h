#ifndef _BATTLEPLAYER_H
#define _BATTLEPLAYER_H
#include "Const.h"
class BattlePlayer
{
public:
	BattlePlayer();
	~BattlePlayer();

	void Initialize(eMusical);
	eMusical mGetType();
private:
	eMusical m_type;
};

#endif