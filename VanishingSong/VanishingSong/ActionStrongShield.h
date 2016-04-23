#pragma once
#include "ActionCommand.h"
class ActionStrongShield :
	public ActionCommand
{
public:
	ActionStrongShield():ActionCommand(eCommandType::eStrongShield){}
	~ActionStrongShield(){}
	void mOnCreate()override{

	}
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override{

	}
};

