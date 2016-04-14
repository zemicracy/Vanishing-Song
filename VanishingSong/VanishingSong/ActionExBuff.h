#pragma once
#include "ActionCommand.h"
class ActionExBuff :
	public ActionCommand
{
public:
	ActionExBuff() :ActionCommand(eCommandType::eSkill){}
	~ActionExBuff(){}

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount)override{

	}
};

