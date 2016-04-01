#pragma once
#include "EnemyAI.h"
#include "CharaStatus.h"
#include "CharaEntity.h"

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
	CharaEntity GetCharaEntity();
	virtual ~EnemyBase()=default;

private:
	virtual bool mSetUp() = 0;

public:
	CharaStatus m_status;
	EnemyAI m_pAI;
	eEnemyType m_type;
	CharaEntity m_charaEntity;
};

