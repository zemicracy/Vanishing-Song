#pragma once
#include "ActionCommand.h"
class ActionAdlib :public ActionCommand
{
public:
	ActionAdlib() :
		ActionCommand(eMusical::eAdlib){}
	~ActionAdlib() = default;
	void mOnCreate()override{
	}
	
	void mOnReset()override{}
};

