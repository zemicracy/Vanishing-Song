#pragma once
#include "EnemyAI.h"
#include "CharaStatus.h"

class EnemyBase
{
private:
	enum class eEnemyType{
		eBoss,
		eAir,
		eGround,
		eNull
	};

public:
	EnemyBase();
	virtual ~EnemyBase()=default;

private:
	virtual bool SetUp() = 0;

public:
	CharaStatus m_status;
	EnemyAI m_pAI;
	eEnemyType m_type;
};

