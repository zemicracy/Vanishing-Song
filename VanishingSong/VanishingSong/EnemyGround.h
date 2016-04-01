#pragma once
#include "EnemyBase.h"
class EnemyGround :
	private EnemyBase
{
public:
	EnemyGround();
	~EnemyGround();
	bool mInitialize(aetherClass::ViewCamera*);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*);
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
};

