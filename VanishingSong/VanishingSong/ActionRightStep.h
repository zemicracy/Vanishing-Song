#pragma once
#include "ActionCommand.h"
class ActionRightStep :
	public ActionCommand
{
public:
	ActionRightStep():ActionCommand(eCommandType::eRightStep){}
	~ActionRightStep(){}
	void mOnCreate()override{

	}
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override{

	}
};

