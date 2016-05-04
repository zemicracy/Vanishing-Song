#pragma once
#include "ActionCommand.h"
class ActionStrongShield :
	public ActionCommand
{
public:
	ActionStrongShield():ActionCommand(eCommandType::eStrongShield){}
	~ActionStrongShield(){}
	void mOnCreate()override{

	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{

	}
	void mOnReset()override{}
};

