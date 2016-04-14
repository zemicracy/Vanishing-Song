#pragma once
#include "ActionCommand.h"
class ActionExAttack :
	public ActionCommand
{
public:
	ActionExAttack();
	~ActionExAttack();

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount)override;
};

