#pragma once
#include "ActionCommand.h"
class ActionNull :public ActionCommand
{
public:
	ActionNull() :
		ActionCommand(eMusical::eNull){}
	~ActionNull() = default;
	void mOnCreate()override{
	}
	
	void mOnReset()override{}
};

