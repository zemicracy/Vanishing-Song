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
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{
		mIsEnd(true);
	}
	void mOnReset()override{}
};

