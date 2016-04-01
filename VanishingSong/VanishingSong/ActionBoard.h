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
	std::shared_ptr<ActionCommand> SelectType();
	
private:
	std::array<std::shared_ptr<ActionCommand>, 7>m_actionCommand;
	
};

