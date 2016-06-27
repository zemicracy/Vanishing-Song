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

	//時間カウント用変数
	m_clockCount = 0;

	//配列番号の切り換え用変数
	m_array = 0;

	//テクスチャ表示繰り返し用変数
	m_imageCount = 0;

	//テクスチャを読み込み
	for (int i = 0; i<17; i++){
		m_pTexture[i] = std::make_unique<Texture>();
		m_pTexture[i]->Load("Texture/Opening/Opening" + std::to_string(i + 1) + ".png");
	}

	m_skipTexture.Load("Texture\\Skip\\skip.png");
	m_pSkip = std::make_shared<Rectangle2D>();
	m_pSkip->Initialize();
	m_pSkip->SetTexture(&m_skipTexture);
	m_pSkip->property._transform._scale = Vector3(150, 100, 0);
	const float x = m_pSkip->property._transform._scale._x;
	const float y = m_pSkip->property._transform._scale._y;
	m_pSkip->property._transform._translation = Vector3(kWindowWidth - x, kWindowHeight - y, 0);
	//1枚目はすぐ表示
	m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
	m_isInit = false;
	return true;
}

void SceneOpening::Finalize(){
	if (m_pSpriteBase){
		m_pSpriteBase->Finalize();
	}
	if (m_pSkip){
		m_pSkip->Finalize();
		m_pSkip.reset();
	}
	return;
}

bool SceneOpening::Updater(){
	if (GameController::GetKey().KeyDownTrigger(VK_RETURN) || GameController::GetJoypad().ButtonPress(eJoyButton::eStart)){
		ChangeScene(SceneGame::Name, LoadState::eUse);
		return true;
	}

	m_bgm.PlayToLoop();
	//m_clockCountにデルタタイムを足していく
	m_clockCount = m_clockCount + GameClock::GetDeltaTime();

	//1、2繰り返し表示
	if (m_imageCount < 2){
		if (m_clockCount > 1){
			m_clockCount = 0;
			if (m_array < 2){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
			}
		}
		//m_arrayが2以上なら
		if (m_array>1){
			m_array = 0;
			m_imageCount++;
		}
	}
	if (m_imageCount == 2){
		m_array = 2;
	}

	//3～17表示
	if (m_imageCount > 1){
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