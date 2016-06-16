#pragma once
#include"MessageWindow.h"
enum class eTutorialState{
	eInit, eFirstDemo, eFirstPlay, eAdlibDemo, eAdlibPlay, ePlayerOnly, eFin
};

class TutorialMessage
{
public:
	TutorialMessage();
	~TutorialMessage();

	bool mInitialize();
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdate();

	void mShowBackCover(const bool);
	void mSetVisible(const bool);
	bool mChangeText(std::string);
private:
	void mFinalize();

	std::unordered_map<std::string, std::string>m_pTextTextureList;
	std::shared_ptr<aetherClass::Texture>m_pTexture;
	std::shared_ptr<MessageWindow>m_message;
	std::shared_ptr<aetherClass::Rectangle2D>m_windowBackCover;
	bool m_isVisible;
	bool m_isShowCover;

};

