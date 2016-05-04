#pragma once
#include "ActionCommand.h"
class ActionNull :public ActionCommand
{
public:
	ActionNull() :
		ActionCommand(eCommandType::eNull){}
	~ActionNull() = default;
	void mOnCreate()override{

	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{}
	void mOnReset()override{}
};

