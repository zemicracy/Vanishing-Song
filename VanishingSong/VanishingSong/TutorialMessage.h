#pragma once
#include"MessageWindow.h"
namespace Tutorial{
	static std::string k_TutorialKey[12] = { "Init", "FirstPreListen", "FirstPerform", "FirstPrePerform", "FirstBattle", "FirstPreBattle",
		"AdlibListen", "AdlibStop", "AdlibPerform", "AdlibBattle", "PlayerOnly", "Fin" };
	enum class eTutorialState{
		eInit, eFirst, eAdlib, ePlayerOnly, eFin
	};
}
class TutorialMessage
{
public:

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
	std::shared_ptr<aetherClass::Texture>m_pButtonTex;
	std::shared_ptr<MessageWindow>m_message;
	std::shared_ptr<aetherClass::Rectangle2D>m_windowBackCover;
	bool m_isVisible;
	bool m_isShowCover;

};

