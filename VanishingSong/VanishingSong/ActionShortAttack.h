#pragma once
#include "ActionCommand.h"
class ActionShortAttack :
	public ActionCommand
{
public:
	ActionShortAttack():ActionCommand(eCommandType::eShortDistanceAttack){}
	~ActionShortAttack(){}
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override{

	}

};

