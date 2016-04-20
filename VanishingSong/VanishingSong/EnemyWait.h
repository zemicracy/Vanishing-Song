#pragma once
#include "EnemyAI.h"
class EnemyWait :
	public EnemyAI
{
public:
	EnemyWait();
	~EnemyWait();

	void UpdateRun(EnemyBase::Property*, aetherClass::Vector3*);

};

