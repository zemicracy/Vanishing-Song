#pragma once
#include "ActionCommand.h"
class ActionStrongShield :
	public ActionCommand
{
public:
	ActionStrongShield():ActionCommand(eCommandType::eStrongShield){}
	~ActionStrongShield(){}
	void mOnCreate()override{
		m_baseUseMP = 5;

	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int count)override{
		mIsEnd(true);
	}
	void mOnReset()override{}
};

