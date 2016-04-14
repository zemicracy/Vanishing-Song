#pragma once
#include "ActionCommand.h"
class ActionExAttack :
	public ActionCommand
{
public:
	ActionExAttack() :ActionCommand(eCommandType::eSkill){}
	~ActionExAttack(){}

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount)override{

	}
};

