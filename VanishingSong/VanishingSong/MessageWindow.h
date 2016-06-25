#pragma once

#include<memory>
#include<Rectangle2D.h>
class MessageWindow
{
	
public:
	MessageWindow();
	~MessageWindow();

	bool mInitialize();
	void mSetText(aetherClass::Texture*);
	
	
	void mRender(aetherClass::ShaderBase*);
	void mSetButton(aetherClass::Texture*);
	void mSetIcon(aetherClass::Texture*);
	void mUpdate(const bool);
private:
	void mFinalize();
	std::shared_ptr<aetherClass::Rectangle2D>m_window;
	std::shared_ptr<aetherClass::Rectangle2D>m_text;
	std::shared_ptr<aetherClass::Rectangle2D>m_button;
	std::shared_ptr<aetherClass::Rectangle2D>m_icon;
	std::shared_ptr<aetherClass::Rectangle2D>m_iconFlame;

	std::shared_ptr<aetherClass::Texture>m_texture;
	std::shared_ptr<aetherClass::Texture> m_iconFlameTexture;

	float m_buttonTime;
	bool m_buttonChange;
};

