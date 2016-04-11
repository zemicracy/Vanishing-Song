#pragma once
#include "ActionCommand.h"
class ActionLongAttack :
	public ActionCommand
{
public:
	ActionLongAttack();
	~ActionLongAttack();
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override;
};

