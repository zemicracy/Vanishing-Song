#pragma once
#include <string>
#include <ShaderBase.h>
#include "MessageWindow.h"
#include <FbxModel.h>
class TutorialEnemy
{
public:
	TutorialEnemy();
	~TutorialEnemy();
	void mInitalize(const bool);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*);
	void mUIRender(aetherClass::ShaderBase*);
	std::string mGetDataPath();
	void mIsEnd(const bool);
private:
	std::string m_dataPath;
	bool m_isEnd;
};

