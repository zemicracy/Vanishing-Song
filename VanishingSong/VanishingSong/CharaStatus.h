#pragma once
#include <array>
namespace{
	const int kMaxOrderList = 5;
	const int kZeroPoint = 0;
}
enum class eCommandType
{
	eShortDistanceAttack,
	eLongDistanceAttack,
	eRightStep,
	eLeftStep,
	eShield,
	eStrongShield,
	eSkill,
	eNull
};

enum class eActionType
{
	eMove,
	eWait,
	eAttack,
	eHitDamage,
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

	float _hp;
	float _mp;
	int _maxhp;
	int _maxmp;
	unsigned int _level;
	int _experiencePoint;
	int _attackPoint;
	int _shieldPoint;
	eBuffType _buff;
	eCommandType _command;
	eActionType _action;
	std::array<eCommandType, kMaxOrderList> _prevCommandList;

	// èâä˙âªóp
	void Reset(){
		_hp = kZeroPoint;
		_mp = kZeroPoint;
		_maxhp = kZeroPoint;
		_maxmp = kZeroPoint;
		_buff = eBuffType::eNull;
		_level = kZeroPoint;
		_experiencePoint = kZeroPoint;
		_attackPoint = kZeroPoint;
		_shieldPoint = kZeroPoint;
		_command = eCommandType::eNull;
		_action = eActionType::eNull;
		_prevCommandList.fill(eCommandType::eNull);
	}
};

