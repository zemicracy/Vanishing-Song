#pragma once
#include <Rectangle2D.h>
#include <GameSound.h>
#include <memory>
#include <array>
#include "MessageWindow.h"
class Config
{
private:

	enum eStae{
		eVolume = 0,
		eSave = 2,
		eBackToTitle = 3,
		eClose = 4,
	};

	enum eVolumeState{
		eLow = 0,
		eNormal,
		eHigh,
		
		
	};
public:
	Config();
	~Config();
	void mIntialize(std::string);
	bool mUpdate(const bool isView, const bool isButton,const std::pair<bool, bool> UpOrDown, const std::pair<bool, bool> RightOrLeft);

	void mUIRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	
	bool mGetIsBackToTitle()const;
private:
	void mFinalize();
	void mCountCursorY(int&, const std::pair<bool,bool>);
	void mCountCursorX(int&, const std::pair<bool, bool>);

	void mMenuDecision(const bool, const int);
	void mVolumeDecision(const bool, const int);
private:

	bool m_isView;
	bool m_isSelectVolume;
	bool m_isBackToTitle;
	int m_cursorYCount;
	int m_cursorXCount;

	std::shared_ptr<aetherClass::Rectangle2D> m_base;
	std::shared_ptr<aetherClass::Rectangle2D> m_cursor;
	aetherClass::Texture m_saveMessage;
	aetherClass::Texture m_pushButton;
	MessageWindow m_messageWindow;
	std::array<float, 5> m_cursorYArray;
	std::array<float, 3> m_cursorXArray;
	aetherClass::Texture m_menuTexture;
	aetherClass::Texture m_volumeTexture;
	std::string m_sceneName;
	aetherClass::GameSound m_selectSE;
	aetherClass::GameSound m_returnSE;
	bool m_isSaved;
	std::pair<aetherClass::Vector3, aetherClass::Vector3> m_cursorScale;
};

