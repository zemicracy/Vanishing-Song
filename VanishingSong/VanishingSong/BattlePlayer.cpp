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

// 自分のタイプを取得
eMusical BattlePlayer::mGetType(){
	return m_type;
}