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

	
	void GetSetEnemyAction() ;
	bool mInitialize(aetherClass::ViewCamera*) override;
	void mInitializeEnemyColider(aetherClass::ViewCamera*,std::shared_ptr<aetherClass::Cube>& collider) override;
	void mUpdate() override;
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*)override;
	bool mSetUp() override;
	void mFinalize() override;
	std::shared_ptr<EnemyAI> GetAI();


private:

	std::shared_ptr<EnemyAI> m_AI;
	std::shared_ptr<aetherClass::Cube> m_pCollider;
	
};
#endif