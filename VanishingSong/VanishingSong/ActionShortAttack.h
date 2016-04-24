#pragma once
#include "ActionCommand.h"

class ActionShortAttack :
	public ActionCommand
{
public:
	ActionShortAttack():ActionCommand(eCommandType::eShortDistanceAttack){}
	~ActionShortAttack(){}
	void mOnCreate()override{

	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& gearFrame, float timeScale, int count)override{

		aetherClass::Vector3 rotation;
		rotation = gInterpolation<aetherClass::Vector3>(aetherClass::Vector3(0, 10, 0), aetherClass::Vector3(20, 10, 0), 60, count);
		
	}

};

