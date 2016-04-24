#pragma once
#include "ActionCommand.h"
class ActionExBuff :
	public ActionCommand
{
public:
	ActionExBuff() :ActionCommand(eCommandType::eSkill){}
	~ActionExBuff(){}
	void mOnCreate()override{

	}

	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int frameCount)override{

	}
};

