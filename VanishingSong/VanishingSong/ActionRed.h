#pragma once
#include "ActionCommand.h"
class ActionRed :public ActionCommand
{
public:
	ActionRed() :
		ActionCommand(eCommandType::eRed){}
	~ActionRed() = default;
	void mOnCreate()override{
	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{
		mIsEnd(true);
	}
	void mOnReset()override{}
};

