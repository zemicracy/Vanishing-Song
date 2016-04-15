#pragma once
#include "ActionCommand.h"

class ActionShortAttack :
	public ActionCommand
{
public:
	ActionShortAttack():ActionCommand(eCommandType::eShortDistanceAttack){}
	~ActionShortAttack(){}
	void mAction(std::shared_ptr<GearFrame> gearFrame, float timeScale, int count)override{

		aetherClass::Vector3 rotation;
		rotation = gInterpolation<aetherClass::Vector3>(aetherClass::Vector3(0, 10, 0), aetherClass::Vector3(20, 10, 0), 60, count);
		mGetCharaEntity().mGearKeyframeTranslation(gearFrame->m_pLeftUpperArm, rotation);
	}

};

