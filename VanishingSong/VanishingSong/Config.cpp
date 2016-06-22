#include "Config.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "Const.h"
#include "PlayDataManager.h"
using namespace aetherClass;
namespace{
	const float kBaseSize = 500.0f;
	const float kDepth = 0.000001f;
	const int kMenuNum = 5;
	const int kSkipNumber = 1;
	const int kTitleSkipNumber = 3;
	const float kLowVolume = -3000.0f;
	const float kNormalVolume = -1500.0f;
	const float kHighVolume = 0.0f;
	const float kVolumeNum = 3;

}

void Config::mIntialize(std::string name){
	m_base = std::make_unique<Rectangle2D>();
	m_base->Initialize();

	m_base->property._transform._scale = Vector3(kBaseSize, kBaseSize, 0);
	m_base->property._transform._translation = Vector3(kWindowWidth / 2 - kBaseSize / 2, kWindowHeight / 2-kBaseSize / 2, kDepth);
	m_base->property._color = Color(0, 0, 0, 1);
	

	m_cursor = std::make_unique<Rectangle2D>();
	m_cursor->Initialize();

	m_cursor->property._transform._scale = Vector3(kBaseSize, kBaseSize/kMenuNum, 0);
	m_cursor->property._transform._translation = m_base->property._transform._translation;
	m_cursor->property._color = Color(1, 1, 1, 1);
	m_cursorScale.first = m_cursor->property._transform._scale;
	m_cursorScale.second = Vector3(kBaseSize / kVolumeNum, kBaseSize / kMenuNum, 0);
	int count = NULL;
	for (auto& index : m_cursorYArray){
		
		index = m_cursor->property._transform._translation._y + (count*(kBaseSize / kMenuNum));
		count += 1;
	}

	count = NULL;
	for (auto& index : m_cursorXArray){
		index = m_cursor->property._transform._translation._x + (count*(kBaseSize / kVolumeNum));
		count += 1;
	}

	//
	m_returnSE.Load("Sound\\Title\\decision.wav");

	//
	m_selectSE.Load("Sound\\Title\\select.wav");

	if (name == SceneGame::Name){

	}
	else if (name == SceneTitle::Name){

	}
	m_sceneName = name;

	
	m_isView = false;
	m_isSelectVolume = false;
	m_isBackToTitle = false;
	m_cursorYCount = NULL;
	m_cursorXCount = NULL;
}

//
bool Config::mUpdate(const bool isView, const bool isButton,const std::pair<bool, bool> UpOrDown, const std::pair<bool, bool> RightOrLeft){
	if (isView){
		m_isView = !m_isView;
	}

	if (!m_isView) return false;

	const float volume = GameManager::mGetInstance().mGetVolume();
	m_returnSE.SetValume(volume);
	m_selectSE.SetValume(volume);

	if (m_isSelectVolume){
		m_cursor->property._transform._scale = m_cursorScale.second;
		m_cursor->property._transform._translation._y = m_cursorYArray.at(kSkipNumber);
		mCountCursorX(m_cursorXCount, RightOrLeft);
		m_cursor->property._transform._translation._x = m_cursorXArray.at(m_cursorXCount);
		mVolumeDecision(isButton, m_cursorXCount);
	}
	else{
		m_cursor->property._transform._scale = m_cursorScale.first;
		m_cursor->property._transform._translation._x = m_cursorXArray.at(0);
		mCountCursorY(m_cursorYCount, UpOrDown);
		m_cursor->property._transform._translation._y = m_cursorYArray.at(m_cursorYCount);
		mMenuDecision(isButton, m_cursorYCount);
		
	}
	
	return true;
}

//
void Config::mUIRender(aetherClass::ShaderBase* tex, aetherClass::ShaderBase* color){
	if (!m_isView) return;
	m_base->Render(color);
	m_cursor->Render(color);
}


//
void Config::mFinalize(){
	m_returnSE.Stop();
	m_selectSE.Stop();
	if (m_base){
		m_base->Finalize();
		m_base.reset();
	}

	if (m_cursor){
		m_cursor->Finalize();
		m_cursor.reset();
	}
}


void Config::mCountCursorY(int& count, const std::pair<bool, bool> UpOrDown){
	if (UpOrDown.second){
		m_selectSE.Stop();
		m_selectSE.PlayToOneTime();
		count += 1;
	}
	else if (UpOrDown.first){
		m_selectSE.Stop();
		m_selectSE.PlayToOneTime();
		count -= 1;
	}

	if (count == kSkipNumber || (m_sceneName == SceneTitle::Name&&count == kTitleSkipNumber)){
		if (UpOrDown.second){
			count += 1;
		}
		else if (UpOrDown.first){
			count -= 1;
		}
	}

	const int size = m_cursorYArray.size();
	if (count >= size){
		count = 0;
	}
	else if (count < 0){
		count = size - 1;
	}

	return;
}


//
void Config::mCountCursorX(int& count, const std::pair<bool, bool> RightOrLeft){
	if (RightOrLeft.first){
		m_selectSE.Stop();
		m_selectSE.PlayToOneTime();
		count += 1;
	}
	else if (RightOrLeft.second){
		m_selectSE.Stop();
		m_selectSE.PlayToOneTime();
		count -= 1;
	}

	const int size = m_cursorXArray.size();
	if (count >= size){
		count = 0;
	}
	else if (count < 0){
		count = size - 1;
	}

	return;
}

//
void Config::mMenuDecision(const bool isButton, const int count){
	if (!isButton)return;

	m_returnSE.Stop();
	m_returnSE.PlayToOneTime();

	PlayDataManager playData;

	switch (count)
	{
	case eStae::eVolume:
		m_isSelectVolume = true;
		break;

	case eStae::eSave:
		playData.mSave();
		playData.mConfigSave();
		break;

	case eStae::eBackToTitle:
		m_isBackToTitle = true;
		break;

	case eStae::eClose:
		m_isView = false;
		break;
	default:
		break;
	}
}

//
void Config::mVolumeDecision(const bool isButton, const int count){
	
	if (!isButton)return;

	m_returnSE.Stop();
	m_returnSE.PlayToOneTime();

	switch (count)
	{
	case eVolumeState::eLow:
		GameManager::mGetInstance().mSetVolume(kLowVolume);
		break;

	case eVolumeState::eNormal:
		GameManager::mGetInstance().mSetVolume(kNormalVolume);
		break;

	case eVolumeState::eHigh:
		GameManager::mGetInstance().mSetVolume(kHighVolume);
		break;
	default:
		break;
	}
	m_isSelectVolume = false;
	PlayDataManager config;
	config.mConfigSave();
}

bool Config::mGetIsBackToTitle()const{
	return m_isBackToTitle;
}