#include "TutorialEnemy.h"
#include "Debug.h"
#include "GameManager.h"
#include <WorldReader.h>
using namespace aetherClass;
TutorialEnemy::TutorialEnemy()
{
	m_dataPath = "data\\Battle\\Tutorial";
	m_isEnd = true;
}


TutorialEnemy::~TutorialEnemy(){
	Finalize();
}

//
void TutorialEnemy::mInitalize(const bool flg,std::shared_ptr<FbxModel>& model){
	m_isEnd = !flg;
	m_model = model;
	WorldReader reader;
	reader.Load("data\\Field\\player_init",true);
	for (auto& index : reader.GetInputWorldInfo()._object){
		if (index->_name == "tutorial_enemy"){
			m_model->property._transform._translation = Vector3(index->_transform._translation._x, 0, index->_transform._translation._z);
			m_model->property._transform._rotation = index->_transform._rotation;
			m_model->property._transform._scale._x = -1;
			m_initTrans = m_model->property._transform;
		}
	}
	reader.UnLoad();
	m_messageWindow.mInitialize();
	
	int count = NULL;
	for (auto& message : m_tutorialMessage){
		
		message = "Texture\\Message\\tmplate.png";
		count += 1;
	}

	//
	count = 0;
	for (auto& message : m_tutorialClearMessage){
		message="Texture\\Message\\tmplate.png";
		count += 1;
	}

	m_pCursor = std::make_shared<Rectangle2D>();
	m_pCursor->Initialize();
	m_pCursor->property._transform._translation._y = 630;
	m_pCursor->property._transform._scale = Vector3(120, 50, 0);
	m_pCursor->property._color = Color(1.f, 1.f, 1.f, 0.3f);

	m_cursorPosition[true] = 400.f;
	m_cursorPosition[false] = 730.f;


	m_pCursor->property._transform._translation._x = m_cursorPosition[m_isYes];

	m_messageCount = NULL;
	m_messageEnd = false;
	m_select = eSelect::eNull;
	m_state = eState::eNext;
	m_isYes = true;
	m_texture[eState::eSelect] = std::make_shared<Texture>();
	m_texture[eState::eSelect]->Load("Texture\\Message\\yesno.png");
	m_texture[eState::eNext] = std::make_shared<Texture>();
	m_texture[eState::eNext]->Load("Texture\\Message\\nextButton.png");

	m_buttonSE.first.Load("Sound\\Field\\message.wav");
	m_buttonSE.second.Load("Sound\\Field\\select.wav");

	const float volume = GameManager::mGetInstance().mGetVolume();
	m_buttonSE.first.SetValume(volume);
	m_buttonSE.second.SetValume(volume);
	
	if (m_isEnd){
		Finalize();
	}
}


//
void TutorialEnemy::mUpdate(const bool isTutorialEnd, const bool selectButton, const bool pushButton){
	const float volume = GameManager::mGetInstance().mGetVolume();
	m_buttonSE.first.SetValume(volume);
	m_buttonSE.second.SetValume(volume);
	if (m_isEnd){
		if (m_pCursor){
			m_pCursor->Finalize();
			m_pCursor.reset();
		}
		return;
	}
	m_model->KeyframeUpdate("wait", true);
	m_model->property._transform = m_initTrans;
	if (selectButton&&m_state == eState::eSelect){
		m_buttonSE.second.Stop();
		m_buttonSE.second.PlayToOneTime();

		m_isYes = !m_isYes;
	}
	m_pCursor->property._transform._translation._x = m_cursorPosition[m_isYes];
	if (pushButton){
		m_buttonSE.first.Stop();
		m_buttonSE.first.PlayToOneTime();

		switch (m_state)
		{
		case eState::eSelect:
			if (m_isYes){
				m_select = eSelect::eYes;

			}
			else{
				m_select = eSelect::eNo;
				m_messageCount += 1;
			}
			m_state = eState::eNext;
			break;

		case eState::eNext:
			m_messageCount += 1;
			break;
		default:
			break;
		}
	}


	if (isTutorialEnd){
		if (m_tutorialClearMessage.size() <= m_messageCount){
			m_messageCount = m_tutorialClearMessage.size() - 1;
			m_messageEnd = true;
		}
		m_message = std::make_shared<Texture>();
		m_message->Load(m_tutorialClearMessage.at(m_messageCount));
		m_messageWindow.mSetText(m_message.get());
	}
	else{
		if (m_tutorialMessage.size() <= m_messageCount){
			m_messageCount = m_tutorialClearMessage.size() - 1;
			m_messageEnd = true;
		}

		if (m_messageCount == m_tutorialMessage.size() - 2){
			m_state = eState::eSelect;
		}

		m_message = std::make_shared<Texture>();
		m_message->Load(m_tutorialMessage.at(m_messageCount));
		m_messageWindow.mSetText(m_message.get());
	}
}

//
void TutorialEnemy::mRender(ShaderBase* tex){
	if (m_isEnd)return;
	m_model->KeyframeAnimationRender(tex);
}

void TutorialEnemy::mUIRender(ShaderBase* shader, ShaderBase* color){
	if (m_isEnd)return;
	m_messageWindow.mSetButton(m_texture[m_state].get());
	m_messageWindow.mRender(shader);
	
	if (m_state == eState::eSelect){
		m_messageWindow.mUpdate(true);
		m_pCursor->Render(color);
	}
	else{
		m_messageWindow.mUpdate(false);
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

void TutorialEnemy::Finalize(){
	if (m_model){
		m_model.reset();
	}
	if (m_pCursor){
		m_pCursor->Finalize();
		m_pCursor.reset();
	}

	for (auto& index : m_texture){
		if (!index.second)continue;
		index.second.reset();
		index.second = nullptr;
	}
}