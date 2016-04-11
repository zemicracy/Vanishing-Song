#ifndef _ENEMYGROUND_H
#define _ENEMYGROUND_H
#include "EnemyBase.h"
#include "EnemyAI.h"
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
	void mChangeAction() override;
	void mFinalize() override;

	std::shared_ptr<EnemyAI> GetAI();



private:

	std::shared_ptr<EnemyAI> m_AI;
	
};
#endif