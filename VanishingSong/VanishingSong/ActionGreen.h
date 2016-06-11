#pragma once
#include "ActionCommand.h"
class ActionGreen :public ActionCommand
{
public:
	ActionGreen() :
		ActionCommand(eMusical::eGreen){}
	~ActionGreen() = default;
	void mOnCreate()override{
	}
	
	void mOnReset()override{}
};

