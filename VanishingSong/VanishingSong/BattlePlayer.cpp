#include "BattlePlayer.h"


BattlePlayer::BattlePlayer()
{
}


BattlePlayer::~BattlePlayer()
{
}

void BattlePlayer::Initialize(eMusical type){
	m_type = type;
	return;
}

// Ž©•ª‚Ìƒ^ƒCƒv‚ðŽæ“¾
eMusical BattlePlayer::mGetType(){
	return m_type;
}