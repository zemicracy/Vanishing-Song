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
	
	void mFinalize();
	void mRender(aetherClass::ShaderBase*);
	void mSetButton(aetherClass::Texture*);
	void mUpdate();
private:
	std::shared_ptr<aetherClass::Rectangle2D>m_window;
	std::shared_ptr<aetherClass::Rectangle2D>m_text;
	std::shared_ptr<aetherClass::Rectangle2D>m_button;
	std::shared_ptr<aetherClass::Texture>m_texture;
	float m_buttonTime;
	bool m_buttonChange;
};

