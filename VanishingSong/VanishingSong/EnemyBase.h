#ifndef _ENEMYBASE_H
#define _ENEMYBASE_H

#include "CharaStatus.h"
#include "CharaEntity.h"
#include "GearFrame.h"
#include <Transform.h>

class EnemyBase
{

public:
	enum class eEnemyType{
		eBoss,
		eAir,
		eGround,
		eNull
	};
	
	struct Property{
		aetherClass::Transform _transform;	//ˆÊ’u
		std::shared_ptr<GearFrame> _enemy;
	};


public:
	EnemyBase();
	virtual ~EnemyBase()=default;
	CharaEntity GetCharaEntity();
	CharaStatus& GetCharaStatus();
	Property& GetProperty();
	void GetSetEnemyAction();


	virtual bool mInitialize(aetherClass::ViewCamera*) = 0;
	virtual void mUpdate() = 0;
	virtual void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*) = 0;
	virtual bool mSetUp() = 0;
	virtual void mChangeAction() = 0;

private:
	Property m_property;
	CharaStatus m_status;
	eEnemyType m_type;
	CharaEntity m_charaEntity;
};
#endif