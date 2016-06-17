#pragma once
#include <string>
#include <ShaderBase.h>
#include "MessageWindow.h"
#include <FbxModel.h>
#include <array>
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
	void mInitalize(const bool);
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
	std::shared_ptr<aetherClass::Texture> m_message;
	std::array<std::string, 4> m_tutorialMessage;
	std::array<std::string, 4> m_tutorialClearMessage;

	bool m_isEnd;
	int m_messageCount;
	bool m_messageEnd;
	eState m_state;
	bool m_isYes;
	eSelect m_select;
	std::shared_ptr<aetherClass::Rectangle2D> m_pCursor;
	std::unordered_map<bool, float> m_cursorPosition;
	std::unordered_map<eState, std::shared_ptr<aetherClass::Texture>> m_texture;
};

