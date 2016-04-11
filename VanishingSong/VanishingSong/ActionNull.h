#pragma once
#include "ActionCommand.h"
class ActionNull :public ActionCommand
{
public:
	ActionNull() :
		ActionCommand(eCommandType::eNull){}
	~ActionNull() = default;
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override{}
};

