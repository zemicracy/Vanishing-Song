#pragma once
#include "ActionCommand.h"
class ActionLeftStep :
	public ActionCommand
{
public:
	ActionLeftStep():ActionCommand(eCommandType::eLeftStep){}
	~ActionLeftStep(){}
	void mOnCreate()override{

	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>, float timeScale, int frame)override{

	}
};

