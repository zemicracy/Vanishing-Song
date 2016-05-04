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

	struct EnemyStatus{
		int _hp;
		int _mp;
		int _attack;
		int _defense;
		int _level;
	};
	
	struct EnemyMoveRange{
		int enemy_max_x;
		int enemy_min_x;
		int enemy_max_z;
		int enemy_min_z;
	};

	struct Property{
		aetherClass::Transform _transform;	//位置
		std::shared_ptr<GearFrame> _penemy;	//Enemy本体
		std::shared_ptr<aetherClass::Cube> _pcolider;	//Enemyコライダー
		aetherClass::Vector3* _colliderPosition;
		aetherClass::Vector3 _nextDirection;
		int _enemyAreaNo;
		bool m_isRender;
		bool _isMoveJudge;
		EnemyMoveRange _enemyMoveRange;
		bool flag;
	};


public:
	EnemyBase();
	virtual ~EnemyBase()=default;
	CharaEntity mGetCharaEntity();
	CharaStatus& mGetCharaStatus();
	Property& mGetProperty();
	EnemyStatus& mGetEnemyStatus();

	virtual void mGetSetEnemyAction() = 0;
	virtual bool mInitialize(aetherClass::ViewCamera*) = 0;
	virtual void mInitializeEnemyColider(aetherClass::ViewCamera*) = 0;
	virtual void mUpdate() = 0;
	virtual void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*) = 0;
	virtual bool mSetUp() = 0;
	virtual void mFinalize() = 0;

private:
	Property m_property;
	CharaStatus m_status;
	EnemyStatus m_enemystatus;
	eEnemyType m_type;
	CharaEntity m_charaEntity;
};
#endif