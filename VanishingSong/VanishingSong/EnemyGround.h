#ifndef _ENEMYGROUND_H
#define _ENEMYGROUND_H
#include "EnemyBase.h"
#include "EnemyAI.h"
#include "CharaStatus.h"
#include "GameController.h"

class EnemyGround :
	public EnemyBase
{
public:
	EnemyGround();
	~EnemyGround();

	void mGetSetEnemyAction() ;
	bool mInitialize(aetherClass::ViewCamera*) override;
	void mInitializeEnemyColider(aetherClass::ViewCamera*) override;
	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);
	void mUpdate() override;
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*)override;
	bool mSetUp() override;
	void mFinalize() override;
	void mEnemyOnHit() override;
	std::shared_ptr<EnemyAI> GetAI();


private:
	std::shared_ptr<EnemyAI> m_AI;
	std::shared_ptr<Gear> m_pTopGear;
	std::shared_ptr<aetherClass::Cube> m_pCollider;

	
};
#endif