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
	void mVisible(bool flg);
	void mFinalize();
	void mRender(aetherClass::ShaderBase*);

private:
	std::shared_ptr<aetherClass::Rectangle2D>m_window;
	std::shared_ptr<aetherClass::Rectangle2D>m_text;
	std::shared_ptr<aetherClass::Texture>m_texture;

	bool m_visible;
};

