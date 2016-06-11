#pragma once
#include "ActionCommand.h"
class ActionMiss :public ActionCommand
{
public:
	ActionMiss() :
		ActionCommand(eMusical::eMiss){}
	~ActionMiss() = default;
	void mOnCreate()override{
	}
	
	void mOnReset()override{}
};

