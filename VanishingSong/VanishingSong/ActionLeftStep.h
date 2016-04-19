#pragma once
#include "ActionCommand.h"
class ActionLeftStep :
	public ActionCommand
{
public:
	ActionLeftStep():ActionCommand(eCommandType::eLeftStep){}
	~ActionLeftStep(){}

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frame)override{

	}
};

