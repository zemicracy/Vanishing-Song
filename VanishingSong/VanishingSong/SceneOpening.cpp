#include "SceneOpening.h"
#include<iostream>
#include<Rectangle2D.h>
#include<GameController.h>
#include"GameManager.h"
#include"ResourceManager.h"
#include"SceneGame.h"

using namespace aetherClass;

const std::string SceneOpening::Name = "Opening";
SceneOpening::SceneOpening():
GameScene(Name,GetManager())
{
}


SceneOpening::~SceneOpening()
{
}

bool SceneOpening::Initialize(){

	RegisterScene(new SceneGame());

	m_pSpriteBase = std::make_unique<Rectangle2D>();
	m_pSpriteBase->Initialize();
	m_pSpriteBase->property._transform._translation = Vector3(0, 0, 0.001);
	m_pSpriteBase->property._transform._translation = Vector3(0, 0, 0);
	m_pSpriteBase->property._transform._scale._x = 1280;
	m_pSpriteBase->property._transform._scale._y = 720;
	m_pSpriteBase->property._color = Color(0, 0, 0, 1);

	//BGM
	m_openingSound = std::make_shared<GameSound>();
	m_openingSound->Load("Sound\\Opening\\Opening.wav");
	m_openingSound->SetValume(GameManager::mGetInstance().mGetVolume());

	//テクスチャ切り替え用に使っている変数の初期化
	m_clockCount = 0;
	m_array = 0;
	m_openingflg = 0;
	m_imageCount = 0;

	//テクスチャを読み込み
	for (int i = 0; i<17; i++){
		m_pTexture[i] = std::make_unique<Texture>();
		m_pTexture[i]->Load("Texture/Opening/Opening" + std::to_string(i + 1) + ".png");
	}

	//1枚目はすぐ表示
	m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
	m_isInit = false;
	return true;
}

void SceneOpening::Finalize(){
	m_pSpriteBase->Finalize();

	return;
}

bool SceneOpening::Updater(){
	//一回だけ呼ばれる処理
	if (!m_isInit){
		m_openingSound->PlayToOneTime();
		m_isInit = true;
	}
	//m_clockCountにデルタタイムを足していく
	m_clockCount = m_clockCount + GameClock::GetDeltaTime();


	//1秒ごとに1、2繰り返し表示
	if (m_imageCount < 3){
		if (m_clockCount > 0.7){
			m_clockCount = 0;
			if (m_array < 2){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
			}
		}
		if (m_array>1){
			m_array = 0;
			m_imageCount++;
		}
	}
	//1、2繰り返し表示を止めるように入れてる処理です
	if (m_imageCount == 3){
		m_array = 2;
	}
	
	//2秒ごとに3～9表示
	if (m_openingflg == 0 && m_imageCount > 2){
		if (m_clockCount > 2){
			m_clockCount = 0;
			if (m_array < 9){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
				m_imageCount++;
			}
		}
	}

	if (m_imageCount == 10){
		m_openingflg = 1;
	}

	//10を1秒表示
	if (m_openingflg == 1 && m_imageCount > 9){
		if (m_clockCount > 1){
			m_clockCount = 0;
			if (m_array < 10){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
				m_imageCount++;
			}
		}
	}
	
	//11、12繰り返し表示するほうに処理がいくように入れてます
	if (m_imageCount == 11){
		m_openingflg = 2;
	}

	//11、12繰り返し表示
	if (m_openingflg == 2 && m_imageCount > 10){
		if (m_clockCount > 0.5){
			m_clockCount = 0;
			if (m_array < 12){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
			}
		}
		if (m_array>11){
			m_array = 10;
			m_imageCount++;
		}
	}

	//11、12繰り返し表示する処理を止めるために入れてます
	if (m_imageCount == 12){
		m_openingflg = 3;
	}

	//13～17表示
	if (m_openingflg == 3){
		if (m_clockCount > 2){
			m_clockCount = 0;
			if (m_array < 17){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
			}
			else{
				ChangeScene(SceneGame::Name, LoadState::eUse);
			}
		}
	}

	return true;
}

void SceneOpening::Render(){
	return;
}

void SceneOpening::UIRender(){
	auto &shaderHash = ResourceManager::mGetInstance().mGetShaderHash();

	m_pSpriteBase->Render(shaderHash["texture"].get());
	return;
}

bool SceneOpening::TransitionIn(){
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		return kTransitionning;
	}
	return kTransitionEnd;
}

bool SceneOpening::TransitionOut(){
	if (!GameManager::mGetInstance().mfadeManager().Out(2)){
		return kTransitionning;
		m_isInit = true;
	}
	m_isInit = false;
	return kTransitionEnd;
}