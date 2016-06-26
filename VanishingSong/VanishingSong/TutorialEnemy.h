#pragma once
#include <string>
#include <ShaderBase.h>
#include "MessageWindow.h"
#include <FbxModel.h>
#include <array>
#include <GameSound.h>
class TutorialEnemy
{
	enum class eState{
		eSelect,
		eNext,
		eEnd
	};

public:
	enum class eSelect{
		eYes,
		eNo,
		eNull
	};
public:
	TutorialEnemy();
	~TutorialEnemy();
	void mInitalize(const bool,std::shared_ptr<aetherClass::FbxModel>& model);
	void Finalize();
	void mUpdate(const bool isTutorialEnd, const bool selectButton, const bool pushButton);
	void mRender(aetherClass::ShaderBase*);
	void mUIRender(aetherClass::ShaderBase*,aetherClass::ShaderBase*);
	std::string mGetDataPath();
	void mIsEnd(const bool);
	bool mGetMessageEnd();

	eSelect mGetSelectType();
private:
	std::string m_dataPath;
	MessageWindow m_messageWindow;
	std::shared_ptr<aetherClass::FbxModel> m_model;
	std::shared_ptr<aetherClass::Texture> m_message;
	std::array<std::string, 5> m_tutorialMessage;
	std::array<std::string, 6> m_tutorialClearMessage;

	aetherClass::Texture m_icon;
	bool m_isEnd;
	bool m_hogeFuga;
	int m_messageCount;
	bool m_messageEnd;
	eState m_state;
	bool m_isYes;
	eSelect m_select;
	std::shared_ptr<aetherClass::Rectangle2D> m_pCursor;
	std::unordered_map<bool, float> m_cursorPosition;
	std::unordered_map<eState, std::shared_ptr<aetherClass::Texture>> m_texture;
	aetherClass::Transform m_initTrans;

	std::pair<aetherClass::GameSound, aetherClass::GameSound> m_buttonSE;
};

