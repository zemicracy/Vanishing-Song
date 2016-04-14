#pragma once
#include "ActionCommand.h"
class ActionExHeel :
	public ActionCommand
{
public:
	ActionExHeel();
	~ActionExHeel();
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount)override;
};

