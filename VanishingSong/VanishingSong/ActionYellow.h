#pragma once
#include "ActionCommand.h"
class ActionYellow :public ActionCommand
{
public:
	ActionYellow() :
		ActionCommand(eMusical::eYellow){}
	~ActionYellow() = default;
	void mOnCreate()override{
	}
	
	void mOnReset()override{}
};

