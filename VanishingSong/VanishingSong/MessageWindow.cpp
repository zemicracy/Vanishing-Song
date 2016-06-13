#include "MessageWindow.h"
#include<WorldReader.h>
#include <GameClock.h>
using namespace aetherClass;
namespace{
	const float kMaxButtonTime = 0.5f;
}
MessageWindow::MessageWindow()
{
	m_text = nullptr;
	m_window = nullptr;
}


MessageWindow::~MessageWindow()
{
	mFinalize();
}

void MessageWindow::mFinalize(){
	if (m_button){
		m_button->Finalize();
		m_button.reset();
	}
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

	m_button = std::make_shared<Rectangle2D>();
	result = m_button->Initialize();
	if (!result){
		return false;
	}

	WorldReader reader;
	reader.Load("data\\MessageWindow.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "window"){
			m_window->property._transform = itr->_transform;
			m_texture = std::make_shared<Texture>();
			m_texture->Load("Texture\\Message\\window.png");
			m_window->SetTexture(m_texture.get());
		}
		if (itr->_name == "textarea"){
			m_text->property._transform = itr->_transform;
		}

		if (itr->_name == "YesOrNo"){
			m_button->property._transform = itr->_transform;
		}
	}
	reader.UnLoad();

	m_buttonTime = NULL;
	m_buttonChange =true;
	return true;
}

void MessageWindow::mSetText(aetherClass::Texture *tex){
	m_text->SetTexture(tex);
}

void MessageWindow::mSetButton(aetherClass::Texture* tex){
	m_button->SetTexture(tex);
}

void MessageWindow::mUpdate(const bool flg){
	if (!m_button)return;
	m_buttonTime += GameClock::GetDeltaTime();
	if (m_buttonTime > kMaxButtonTime){
		m_buttonChange = !m_buttonChange;
		m_buttonTime = NULL;
	}
	if (flg){
		m_button->property._color._alpha = 1.0f;
	}
	else{
		if (m_buttonChange){
			m_button->property._color._alpha = 1.0f;
		}
		else{
			m_button->property._color._alpha = 0.0f;
		}
	}
}

void MessageWindow::mRender(ShaderBase *shader){
	m_window->Render(shader);
	m_text->Render(shader);
	m_button->Render(shader);
	
}