#pragma once
#include "ActionCommand.h"
class ActionRed :public ActionCommand
{
public:
	ActionRed() :
		ActionCommand(eMusical::eRed){}
	~ActionRed() = default;
	void mOnCreate()override{
	}
	
	void mOnReset()override{}
};

