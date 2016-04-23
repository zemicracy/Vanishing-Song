#pragma once
#include "ActionCommand.h"
class ActionExHeel :
	public ActionCommand
{
public:
	ActionExHeel() :ActionCommand(eCommandType::eSkill){}
	~ActionExHeel(){}

	void mOnCreate()override{

	}
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount)override{

	}
};

