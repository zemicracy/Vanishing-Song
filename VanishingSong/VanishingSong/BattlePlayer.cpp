#include "BattlePlayer.h"


BattlePlayer::BattlePlayer()
{
}


BattlePlayer::~BattlePlayer()
{
}

//
void BattlePlayer::mInitialize(eMusical type, std::shared_ptr<GearFrame> gearframe){
	m_type = type;
	m_playerGear = gearframe;
	return;
}

//
void BattlePlayer::mRender(aetherClass::ShaderBase* shader){
	m_charaEntity.mGearRender(m_playerGear->m_pBody, shader, shader);
}

//
void BattlePlayer::mUpdate(const float){
	// �A�j���[�V�����n���ȁH
}

// �����̃^�C�v���擾
eMusical BattlePlayer::mGetType(){
	return m_type;
}

//
void BattlePlayer::mSetPosition(aetherClass::Vector3 position){
	m_transform._translation = position;
}
