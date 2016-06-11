#pragma once
#include "ActionCommand.h"
class ActionBlue :public ActionCommand
{
public:
	ActionBlue() :
		ActionCommand(eMusical::eBlue){}
	~ActionBlue() = default;
	void mOnCreate()override{
	}
	
	void mOnReset()override{}
};

