#ifndef _FADEMANAGER_H
#define _FADEMANAGER_H
#include <SpriteBase.h>
#include <memory>
#include <Rectangle2D.h>
#include <GameClock.h>
#include "Const.h"
class FadeManager
{
public:
	FadeManager(){
		mInitialize();
	}
	~FadeManager(){
		mFinalize();
	}

	
	bool In(float timeScale){
		if (!m_callIn){
			m_pFadeObject->property._color._alpha = 0;
			m_callIn = true;
		}

		m_pFadeObject->property._color._alpha += timeScale*(float)aetherClass::GameClock::GetDeltaTime();
		if (m_pFadeObject->property._color._alpha < 1.2) return false;

		m_callIn = false;
		return true;
	}


	void mRedner(aetherClass::ShaderBase* shader){
		m_pFadeObject->Render(shader);
		return;
	}


	bool Out(float timeScale){
		if (!m_callOut){
			m_pFadeObject->property._color._alpha = 1.0f;
			m_callOut = true;
		}

		m_pFadeObject->property._color._alpha -= timeScale*(float)aetherClass::GameClock::GetDeltaTime();
		if (m_pFadeObject->property._color._alpha > -0.2) return false;

		m_callOut = false;
		return true;
	}
private:

	void mInitialize(){
		// フェードイン・アウト用
		m_pFadeObject = std::make_unique<aetherClass::Rectangle2D>();
		m_pFadeObject->Initialize();
		m_pFadeObject->property._transform._scale = aetherClass::Vector3(kWindowWidth, kWindowHeight, NULL);
		m_pFadeObject->property._color = aetherClass::Color(0, 0, 0, 0);
		m_callIn = false;
		m_callOut = false;
	}


	void mFinalize(){
		if (m_pFadeObject){
			m_pFadeObject->Finalize();
			m_pFadeObject.reset();
			m_pFadeObject = nullptr;
		}
	}
private:
	std::unique_ptr<aetherClass::SpriteBase> m_pFadeObject;

	bool m_callIn;
	bool m_callOut;
};

#endif