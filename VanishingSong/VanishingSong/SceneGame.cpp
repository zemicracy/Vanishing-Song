#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
#include "SceneBattle.h"
#include "ResourceManager.h"
using namespace aetherClass;

const std::string SceneGame::Name = "Game";

namespace{
	const float kScaleTime = 1.0f; 
	const bool kError = false;
	const float kDayTime = 180.0f; // 一日の時間
}

SceneGame::SceneGame():
GameScene(Name, GetManager()) //Sceneごとの名前を設定
{
	m_gameState = eState::eNull;

}


SceneGame::~SceneGame()
{

	m_gameState = eState::eNull;

}

bool SceneGame::Initialize(){
	bool result = true;

	Finalize();

	// シーンの登録
	RegisterScene(new SceneTitle());
	RegisterScene(new SceneBattle());

	// フェードイン・アウトを行う
	m_pFadeObject = std::make_unique<FadeManager>();

	m_pFieldPlayer = std::make_shared<FieldPlayer>();
	m_pFieldPlayer->mInitialize();

	auto view = m_pFieldPlayer->mGetView();
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);
	
	m_pCollideManager = std::make_unique<CollideManager>(m_pFieldPlayer, m_pFieldArea);


	
	// ゲームの状態を登録
	m_gameState = eState::eRun;

	std::cout << "終了" << std::endl;
	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){

	m_gameState = eState::eNull;
	if (m_pCollideManager){
		m_pCollideManager.release();
		m_pCollideManager = nullptr;
	}

	if (m_pFieldArea){
		m_pFieldArea.reset();
		m_pFieldArea = nullptr;
	}

	if (m_pFieldPlayer){
		m_pFieldPlayer.reset();
		m_pFieldPlayer = nullptr;
	}

	if (m_pFadeObject){
		m_pFadeObject.release();
		m_pFadeObject = nullptr;
	}
	return;
}

// 更新処理
bool SceneGame::Updater(){
	if (m_gameState == eState::eFadeIn || m_gameState == eState::eFadeOut){
		bool result = mFadeState(m_gameState);
		if (!result){
			return true;
		}
	}

	if (GameController::GetKey().KeyDownTrigger(VK_ESCAPE)){
		m_gameState = eState::eExit;
	}

	if (GameController::GetKey().KeyDownTrigger('B')){
		m_gameState = eState::eExit;
	}

	if (m_gameState == eState::eExit){
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return true;
	}
	else if (m_gameState == eState::eBattle){
		ChangeScene(SceneBattle::Name, LoadState::eUse);
		return true;
	}

	m_pFieldArea->mUpdate(kScaleTime);
	m_pFieldPlayer->mUpdate(kScaleTime);

	// この処理は最後
	m_pCollideManager->mUpdate();
	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	m_pFieldPlayer->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pFieldArea->mRender(shaderHash["texture"].get(), shaderHash["color"].get());


	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	if (m_gameState == eState::eFadeIn || m_gameState == eState::eFadeOut){
		m_pFadeObject->mRedner(shaderHash["color"].get());
	}
	return;
}

bool SceneGame::TransitionIn(){

	return kTransitionEnd;
}

bool SceneGame::TransitionOut(){

	return kTransitionEnd;
}


//
bool SceneGame::mFadeState(SceneGame::eState state){ 
	if (state == eState::eFadeIn || state == eState::eFadeOut){
		bool isEnd = false;
		switch (state)
		{
		case eState::eFadeIn:
			isEnd = m_pFadeObject->In(1.0f);
			if (isEnd){
				m_gameState = eState::eFadeOut;
			}
			break;

		case eState::eFadeOut:
			isEnd = m_pFadeObject->Out(1.0f);
			if (isEnd){
				m_gameState = eState::eRun;
			}
			break;
		}
		if (!isEnd) return false;
	}
	return true;
}