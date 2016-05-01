#pragma once

#include<array>
#include<ShaderBase.h>
#include"ActionCommand.h"
#include"GameManager.h"
class ActionBoard
{
public:
	ActionBoard();
	~ActionBoard();

	void mInitialize(GameManager::eSkillType);
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(float);
	std::shared_ptr<ActionCommand> mSelectType();
	
private:
	void mChangeGround(int );
	void mChangeAnimation(float,int);

	void mFinalize();
	bool mIntersect(std::shared_ptr<ActionCommand>);
	std::array<std::shared_ptr<ActionCommand>, 7>m_actionCommand;
private:
	float m_frameCnt[3];
	bool m_movingAnimation[3];
	bool m_changedSuccess[3];

	const float m_kForeground = 0;
	const float m_kBackground = 0.5;

};

