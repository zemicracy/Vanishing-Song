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
		aetherClass::Transform _transform;	//�ʒu
		std::shared_ptr<GearFrame> _enemy;
		bool m_isRender;
	};


public:
	EnemyBase();
	virtual ~EnemyBase()=default;
	CharaEntity GetCharaEntity();
	CharaStatus& GetCharaStatus();
	Property& GetProperty();

	virtual void GetSetEnemyAction() = 0;
	virtual bool mInitialize(aetherClass::ViewCamera*) = 0;
	virtual void mInitializeEnemyColider(aetherClass::ViewCamera*,std::shared_ptr<aetherClass::Cube>& collider) = 0;
	virtual void mUpdate() = 0;
	virtual void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*) = 0;
	virtual bool mSetUp() = 0;
	virtual void mFinalize() = 0;

private:
	Property m_property;
	CharaStatus m_status;
	eEnemyType m_type;
	CharaEntity m_charaEntity;
};
#endif