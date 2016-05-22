#ifndef _BATTLEPLAYER_H
#define _BATTLEPLAYER_H
#include "Const.h"
#include "CharaEntity.h"
#include "GearFrame.h"
#include <ShaderBase.h>
class BattlePlayer
{
public:
	BattlePlayer();
	~BattlePlayer();

	void mInitialize(eMusical,std::shared_ptr<GearFrame> gearframe);
	eMusical mGetType();
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(const float);
	void mSetPosition(aetherClass::Vector3);
private:
	eMusical m_type;
	CharaEntity m_charaEntity;
	std::shared_ptr<GearFrame> m_playerGear;
	aetherClass::Transform m_transform;
};

#endif