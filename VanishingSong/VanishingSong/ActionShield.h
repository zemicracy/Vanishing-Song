#pragma once
#include "ActionCommand.h"
class ActionShield :
	public ActionCommand
{
public:
	ActionShield():ActionCommand(eCommandType::eShield){}
	~ActionShield(){}

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override{

	}
};

