#pragma once
#include "EnemyAI.h"
class EnemyDie :
	public EnemyAI
{
public:
	EnemyDie();
	~EnemyDie();

	void mUpdateRun(EnemyBase::Property*,aetherClass::Vector3*)override;
};

