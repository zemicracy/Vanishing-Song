#pragma once
#include "ActionCommand.h"
class ActionStrongShield :
	public ActionCommand
{
public:
	ActionStrongShield();
	~ActionStrongShield();

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override;
};

