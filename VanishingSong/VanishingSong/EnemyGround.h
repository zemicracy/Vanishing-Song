#ifndef _ENEMYGROUND_H
#define _ENEMYGROUND_H

#include "EnemyBase.h"
#include "CharaStatus.h"
#include "GameController.h"

class EnemyGround :
	private EnemyBase
{
public:
	EnemyGround();
	~EnemyGround();

	bool mInitialize(aetherClass::ViewCamera*) override;
	void mUpdate() override;
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*)override;
	bool mSetUp() override;
	void EnemyDie() override;
	void ChangeAction() override;



private:

	std::shared_ptr<GearFrame> m_pGearFrame;
	
};
#endif