#pragma once
#include "ActionCommand.h"
class ActionLeftStep :
	public ActionCommand
{
public:
	ActionLeftStep();
	~ActionLeftStep();

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override;
};

