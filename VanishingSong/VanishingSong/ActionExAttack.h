#pragma once
#include "ActionCommand.h"
class ActionExAttack :
	public ActionCommand
{
public:
	ActionExAttack() :ActionCommand(eCommandType::eSkill){}
	~ActionExAttack(){}
	void mOnCreate()override{
		m_baseUseMP = 5;

	}


	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int frameCount)override{
		mIsEnd(true);
	}


	void mOnReset()override{}
};

