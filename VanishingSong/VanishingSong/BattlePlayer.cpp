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

// �����̃^�C�v���擾
eMusical BattlePlayer::mGetType(){
	return m_type;
}