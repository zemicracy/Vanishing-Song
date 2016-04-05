#ifndef _ENEMYBASE_H
#define _ENEMYBASE_H

#include "CharaStatus.h"
#include "CharaEntity.h"

class EnemyBase
{

public:
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
	CharaStatus& GetCharaStatus();


	virtual bool mInitialize(aetherClass::ViewCamera*) = 0;
	virtual void mUpdate() = 0;
	virtual void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*) = 0;
	virtual bool mSetUp() = 0;
	virtual void EnemyDie() = 0;
	virtual void ChangeAction() = 0;

private:
	CharaStatus m_status;
	eEnemyType m_type;
	CharaEntity m_charaEntity;
};
#endif