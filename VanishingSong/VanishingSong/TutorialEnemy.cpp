#include "TutorialEnemy.h"

using namespace aetherClass;
TutorialEnemy::TutorialEnemy()
{
	m_dataPath = "data\\Battle\\Tutorial";
	m_isEnd = true;
}


TutorialEnemy::~TutorialEnemy(){}

//
void TutorialEnemy::mInitalize(const bool flg){
	m_isEnd = !flg;

	m_messageWindow.mInitialize();
	
	int count = NULL;
	for (auto& message : m_tutorialMessage){
		message.Load("Texture\\Message\\tmplate.png");
		count += 1;
	}

	//
	count = 0;
	for (auto& message : m_tutorialClearMessage){
		message.Load("Texture\\Message\\tmplate.png");
		count += 1;
	}

	m_messageCount = NULL;
	m_messageEnd = false;
}


//
void TutorialEnemy::mUpdate(const bool isTutorialEnd, const bool selectButton, const bool pushButton){
	if (m_isEnd)return;
	if (pushButton){
		m_messageCount += 1;
	}
	if (isTutorialEnd){
		if (m_tutorialClearMessage.size() <= m_messageCount){
			m_messageCount = m_tutorialClearMessage.size() - 1;
			m_messageEnd = true;
		}

		m_messageWindow.mSetText(&m_tutorialClearMessage.at(m_messageCount));
	}
	else{
		if (m_tutorialClearMessage.size() <= m_messageCount){
			m_messageCount = m_tutorialClearMessage.size() - 1;
			m_messageEnd = true;
		}

		m_messageWindow.mSetText(&m_tutorialMessage.at(m_messageCount));
	}
}

//
void TutorialEnemy::mRender(ShaderBase*){
	if (m_isEnd)return;
}

void TutorialEnemy::mUIRender(ShaderBase* shader){
	if (m_isEnd)return;
	m_messageWindow.mRender(shader);
}

//
std::string TutorialEnemy::mGetDataPath(){
	return m_dataPath;
}

void TutorialEnemy::mIsEnd(const bool flg){
	m_isEnd = flg;
}

bool TutorialEnemy::mGetMessageEnd(){
	return m_messageEnd;
}