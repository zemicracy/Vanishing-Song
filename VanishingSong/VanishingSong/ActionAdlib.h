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
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{
		mIsEnd(true);
	}
	void mOnReset()override{}
};
