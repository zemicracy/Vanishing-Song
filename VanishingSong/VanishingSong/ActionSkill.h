#pragma once
#include "ActionCommand.h"
class ActionSkill :
	public ActionCommand
{
public:
	ActionSkill();
	~ActionSkill();

	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override;
};

