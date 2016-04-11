#pragma once
#include"EnemyBase.h"
#include"Player.h"
class EnemyAI
{
public:
	EnemyAI();
	~EnemyAI();

	virtual void UpdateRun(EnemyBase::Property*) = 0;
	
};

