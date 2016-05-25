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
	// ‰Šú‰»—p
	void Reset(){
		_hp = kZeroPoint;
		_maxHp = kZeroPoint;
	}
};

