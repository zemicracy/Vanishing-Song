#pragma once
#include "ActionCommand.h"
class ActionShortAttack :
	public ActionCommand
{
public:
	ActionShortAttack();
	~ActionShortAttack();
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override;

};

