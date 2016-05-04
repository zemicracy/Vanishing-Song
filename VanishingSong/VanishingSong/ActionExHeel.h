#pragma once
#include "ActionCommand.h"
class ActionExHeel :
	public ActionCommand
{
public:
	ActionExHeel() :ActionCommand(eCommandType::eSkill){}
	~ActionExHeel(){}

	void mOnCreate()override{

		m_baseUseMP = 5;
	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int frameCount)override{
		mIsEnd(true);
	}

	void mOnReset()override{}
};

