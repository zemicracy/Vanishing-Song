#pragma once
#include "ActionCommand.h"
class ActionBlue :public ActionCommand
{
public:
	ActionBlue() :
		ActionCommand(eCommandType::eBlue){}
	~ActionBlue() = default;
	void mOnCreate()override{
	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{
		mIsEnd(true);
	}
	void mOnReset()override{}
};

