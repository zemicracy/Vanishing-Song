#pragma once
#include <string>
#include <ShaderBase.h>
#include "MessageWindow.h"
#include <FbxModel.h>
#include <array>
class TutorialEnemy
{
public:
	TutorialEnemy();
	~TutorialEnemy();
	void mInitalize(const bool);
	void mUpdate(const bool isTutorialEnd, const bool selectButton, const bool pushButton);
	void mRender(aetherClass::ShaderBase*);
	void mUIRender(aetherClass::ShaderBase*);
	std::string mGetDataPath();
	void mIsEnd(const bool);
	bool mGetMessageEnd();
private:
	std::string m_dataPath;
	std::shared_ptr<MessageWindow> m_messageWindow;
	std::array<aetherClass::Texture, 4> m_tutorialMessage;
	std::array<aetherClass::Texture, 4> m_tutorialClearMessage;
	bool m_isEnd;
	int m_messageCount;
	bool m_messageEnd;
};

