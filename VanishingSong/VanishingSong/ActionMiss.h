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
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{
		mIsEnd(true);
	}
	void mOnReset()override{}
};

