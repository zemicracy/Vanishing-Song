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
	virtual ~EnemyBase()=default;
	CharaEntity GetCharaEntity();

	virtual bool mInitialize(aetherClass::ViewCamera*) = 0;
	virtual void mUpdate() = 0;
	virtual void mRender(aetherClass::ShaderBase*) = 0;
	virtual bool mSetUp() = 0;

private:
	CharaStatus m_status;
	EnemyAI m_pAI;
	eEnemyType m_type;
	CharaEntity m_charaEntity;
};

