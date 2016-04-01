#pragma once
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
	eNull
};
class CharaStatus
{
public:
	CharaStatus();
	~CharaStatus();
};

