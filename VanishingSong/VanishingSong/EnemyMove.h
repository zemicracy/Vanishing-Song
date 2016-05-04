#pragma once
#include "EnemyAI.h"
class EnemyMove :
	public EnemyAI
{
public:
	EnemyMove();
	~EnemyMove();

	void mUpdateRun(EnemyBase::Property*, aetherClass::Vector3*)override;
	void mTargetMove(EnemyBase::Property*,float,float);

private:
	CharaEntity m_charaEntity;
	int randomValuex;
	int randomValuez;
	bool flag;

};

