#pragma once
#include <array>
namespace{
	const int kMaxOrderList = 5;
	const int kZeroPoint = 0;
}
enum class eActionType
{
	eShortDistanceAttack,
	eLongDistanceAttack,
	eRightStep,
	eLeftStep,
	eShield,
	eStrongShield,
	eSkill,
	eMove,
	eWait,
	eAttack,
	eDie,
	eNull
};

enum class eBuffType{
	eAttackUp,
	eNull,
};

struct CharaStatus
{

	CharaStatus(){
		Reset();
	}
	~CharaStatus(){
		Reset();
	}

	int _hp;
	int _mp;
	unsigned int _level;
	int _experiencePoint;
	int _attackPoint;
	int _shieldPoint;
	eBuffType _buff;
	eActionType _nowAction;
	std::array<eActionType, kMaxOrderList> _prevActionList;

	// èâä˙âªóp
	void Reset(){
		_hp = kZeroPoint;
		_mp = kZeroPoint;
		_buff = eBuffType::eNull;
		_level = kZeroPoint;
		_experiencePoint = kZeroPoint;
		_attackPoint = kZeroPoint;
		_shieldPoint = kZeroPoint;
		_nowAction = eActionType::eNull;
		_prevActionList.fill(eActionType::eNull);
	}
};

