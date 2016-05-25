#pragma once
#include <array>
namespace{
	const int kZeroPoint = 0;
}

enum class eActionType
{
	eMove,
	eWait,
	eNull
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

	float _maxHp;
	// �������p
	void Reset(){
		_hp = kZeroPoint;
		_maxHp = kZeroPoint;
	}
};

