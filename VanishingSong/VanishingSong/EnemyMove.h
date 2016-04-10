#pragma once
#include "EnemyAI.h"
class EnemyMove :
	public EnemyAI
{
public:
	EnemyMove();
	~EnemyMove();

	void UpdateRun(EnemyBase::Property&)override;
};

