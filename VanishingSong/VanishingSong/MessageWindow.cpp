#include "MessageWindow.h"
#include<WorldReader.h>

using namespace aetherClass;
MessageWindow::MessageWindow()
{
	m_text = nullptr;
	m_window = nullptr;
	m_visible = true;
}


MessageWindow::~MessageWindow()
{
	mFinalize();
}

void MessageWindow::mFinalize(){
	if (m_text){
		m_text->Finalize();
		m_text.reset();
	}
	if (m_window){
		m_window->Finalize();
		m_window.reset();
	}
}

bool MessageWindow::mInitialize(){
	
	m_text = std::make_shared<Rectangle2D>();
	bool result = m_text->Initialize();
	if (!result){
		return false;
	}

	m_window = std::make_shared<Rectangle2D>();
	result = m_window->Initialize();
	if (!result){
		return false;
	}

	WorldReader reader;
	reader.Load("data\\MessageWindow.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "window"){
			m_window->property._transform = itr->_transform;
			m_texture = std::make_shared<Texture>();
			m_texture->Load("Texture\\leftStep.png");
			m_window->SetTexture(m_texture.get());
		}
		if (itr->_name == "textarea"){
			m_text->property._transform = itr->_transform;
		}
	}
	reader.UnLoad();

	return true;
}

void MessageWindow::mSetText(aetherClass::Texture *tex){
	m_text->SetTexture(tex);
}

void MessageWindow::mVisible(bool flg){
	m_visible = flg;
}

void MessageWindow::mRender(ShaderBase *shader){
	m_window->Render(shader);

	if (m_visible){
		m_text->Render(shader);
	}
}


