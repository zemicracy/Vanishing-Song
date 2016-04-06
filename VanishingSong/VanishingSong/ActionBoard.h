#pragma once

#include<array>
#include<ShaderBase.h>
#include"ActionCommand.h"
class ActionBoard
{
public:
	ActionBoard();
	~ActionBoard();

	void mInitialize();
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(float);
	std::shared_ptr<ActionCommand> mSelectType();
	
private:
	void mFinalize();
	std::array<std::shared_ptr<ActionCommand>, 7>m_actionCommand;
	const float m_kForeground = 0;
	const float m_kBackground = 0.5;

};

