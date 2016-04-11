#pragma once
#include "EnemyAI.h"
class EnemyDie :
	public EnemyAI
{
public:
	EnemyDie();
	~EnemyDie();

	void UpdateRun(EnemyBase::Property*)override;
};

