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
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{
		mIsEnd(true);
	}
	void mOnReset()override{}
};

