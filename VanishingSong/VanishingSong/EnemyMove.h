#pragma once
#include "EnemyAI.h"
class EnemyMove :
	public EnemyAI
{
public:
	EnemyMove();
	~EnemyMove();

	void mUpdateRun(EnemyBase::Property*, aetherClass::Vector3*)override;
	void mTargetMove(EnemyBase::Property*);

private:
	CharaEntity m_charaEntity;
};

