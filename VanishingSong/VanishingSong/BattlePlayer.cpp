#include "BattlePlayer.h"


BattlePlayer::BattlePlayer()
{
}


BattlePlayer::~BattlePlayer()
{
}

//
void BattlePlayer::mInitialize(eMusical type, aetherClass::Vector3 position, std::shared_ptr<GearFrame> gearframe){
	m_type = type;
	m_playerGear = gearframe;
	m_transform._translation = position;
	return;
}

//
void BattlePlayer::mRender(aetherClass::ShaderBase* shader){
	m_charaEntity.mGearRender(m_playerGear->m_pBody, shader, shader);
}

//
void BattlePlayer::mUpdate(const float){
	// アニメーション系かな？
	m_charaEntity.mGearMove(m_playerGear->m_pBody, m_transform._translation, "=");
}

// 自分のタイプを取得
eMusical BattlePlayer::mGetType(){
	return m_type;
}

//
void BattlePlayer::mSetPosition(aetherClass::Vector3 position){
	m_transform._translation = position;
}
