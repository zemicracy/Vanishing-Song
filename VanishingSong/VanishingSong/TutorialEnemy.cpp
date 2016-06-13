#include "TutorialEnemy.h"
#include "Debug.h"
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

	m_pCursor = std::make_shared<Rectangle2D>();
	m_pCursor->Initialize();
	m_pCursor->property._transform._translation._y = 630;
	m_pCursor->property._transform._scale = Vector3(120, 50, 0);
	m_pCursor->property._color = Color(1.f, 1.f, 1.f, 0.3f);

	m_cursorPosition[true] = 400.f;
	m_cursorPosition[false] = 730.f;

	m_messageCount = NULL;
	m_messageEnd = false;
	m_select = eSelect::eNull;
	m_state = eState::eEnd;
	m_isYes = true;
}


//
void TutorialEnemy::mUpdate(const bool isTutorialEnd, const bool selectButton, const bool pushButton){
	if (m_isEnd)return;
	
	if (m_state == eState::eSelect){
		if (selectButton){
			m_isYes = !m_isYes;
		}
		m_pCursor->property._transform._translation._x = m_cursorPosition[m_isYes];
		if (pushButton){
			if (m_isYes){
				m_select = eSelect::eYes;

			}
			else{
				m_select = eSelect::eNo;
				m_messageCount += 1;
			}
			m_state = eState::eNext;
		}
		return;
	}else
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
		if (m_tutorialMessage.size() <= m_messageCount){
			m_messageCount = m_tutorialClearMessage.size() - 1;
			m_messageEnd = true;
		}

		if (m_messageCount > m_tutorialMessage.size() - 2){
			m_state = eState::eSelect;
			Debug::mPrint("‚«‚½");
		}

		m_messageWindow.mSetText(&m_tutorialMessage.at(m_messageCount));
	}
}

//
void TutorialEnemy::mRender(ShaderBase*){
	if (m_isEnd)return;
}

void TutorialEnemy::mUIRender(ShaderBase* shader, ShaderBase* color){
	if (m_isEnd)return;
	m_messageWindow.mRender(shader);
	if (m_state == eState::eSelect){
		m_pCursor->Render(color);
	}
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

TutorialEnemy::eSelect TutorialEnemy::mGetSelectType(){
	return m_select;
}