#pragma once
#include "ActionCommand.h"
class ActionShield :
	public ActionCommand
{
public:
	ActionShield():ActionCommand(eCommandType::eShield){}
	~ActionShield(){}
	void mOnCreate()override{

	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>, float timeScale, int count)override{

	}
};

