#pragma once
#include "ActionCommand.h"
class ActionExShield :
	public ActionCommand
{
public:
	ActionExShield();
	~ActionExShield();

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount)override;
};

