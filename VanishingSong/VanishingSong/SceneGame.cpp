#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
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
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{

	m_gameState = eState::eNull;
	m_dayTime = NULL;

}

bool SceneGame::Initialize(){
	bool result = true;

	Finalize();

	// シーンの登録
	RegisterScene(new SceneTitle());

	// フェードイン・アウトを行う
	m_pFadeObject = std::make_unique<FadeManager>();

	// ゲームモードの取得
	// このシーンに来ている時点で、サバイバルか、プラクティスが選択されている
	auto mode = Singleton<GameManager>::GetInstance().mGameMode(); 
	
	// スキルの取得
	auto skill = Singleton<GameManager>::GetInstance().mSkillType();
	skill = GameManager::eSkillType::eExHeel;

	// ゲームの状態を登録
	m_gameState = eState::eRun;

	std::cout << "終了" << std::endl;
	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){

	m_gameState = eState::eNull;
	m_dayTime = NULL;
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


	if (m_gameState == eState::eExit){
		// 終了ならタイトルに戻る
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return true;
	}

	// 時刻の取得
	m_dayTime += (float)GameClock::GetDeltaTime();
	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
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