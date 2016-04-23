#pragma once
#include "ActionCommand.h"
class ActionExShield :
	public ActionCommand
{
public:
	ActionExShield() :ActionCommand(eCommandType::eSkill){}
	~ActionExShield(){}
	void mOnCreate()override{

	}
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount)override{

	}
};

