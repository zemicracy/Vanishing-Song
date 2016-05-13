#ifndef _ENEMYAI_H
#define _ENEMYAI_H
#include"EnemyBase.h"

class EnemyAI
{
public:
	EnemyAI();
	~EnemyAI();

	std::shared_ptr<EnemyAI> GetAI();
	virtual void mUpdateRun(EnemyBase::Property*,aetherClass::Vector3*) = 0;

private:
	std::shared_ptr<EnemyAI> m_AI;



};
#endif