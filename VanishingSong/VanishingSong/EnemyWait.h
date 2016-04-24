#pragma once
#include "EnemyAI.h"
class EnemyWait :
	public EnemyAI
{
public:
	EnemyWait();
	~EnemyWait();

	void mUpdateRun(EnemyBase::Property*, aetherClass::Vector3*);

};

