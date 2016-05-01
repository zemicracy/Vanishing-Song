#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
#include "ResourceManager.h"
#include "ModeSurvival.h"
#include "ModePractice.h"
using namespace aetherClass;

const std::string SceneGame::Name = "Game";

namespace{
	const float kDefaultScaleTime = 100.0f; 
	const float kCommandTimeScale = 1.0f;
	const bool kError = false;
	const float kDayTime = 300.0f;
}

SceneGame::SceneGame():
GameScene(Name, GetManager()) //Sceneごとの名前を設定
{

	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{

	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}

bool SceneGame::Initialize(){

	Finalize();

	// シーンの登録
	RegisterScene(new SceneTitle());

	bool result = false;

	// アクションコマンドの初期化
	m_pActionBoard = std::make_unique<ActionBoard>();
	auto skill = Singleton<GameManager>::GetInstance().mSkillType();
	skill = GameManager::eSkillType::eExAttack;
	m_pActionBoard->mInitialize(skill);

	// オーダーリストの初期化
	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize();

	// フェードイン・アウトを行う
	m_pFadeObject = std::make_unique<FadeManager>();

	// ゲームモードの取得
	// このシーンに来ている時点で、サバイバルか、プラクティスが選択されている
	auto nowMode = Singleton<GameManager>::GetInstance().mGameMode(); 
	nowMode = GameManager::eGameMode::eSurvaival;

	
	m_pResultBord = std::make_unique<ResultBord>();
	m_pResultBord->mInitialize(nowMode);
	
	// 現在のモードの取得
	m_pMode = mReturnMode(nowMode);
	m_pMode->mInitialize(m_nowDay);

	// ゲームの状態を登録
	m_gameState = eState::eRun;

	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){
	if (m_pActionBoard){
		m_pActionBoard.release();
		m_pActionBoard = nullptr;
	}

	if (m_pOrderList){
		m_pOrderList.release();
		m_pOrderList = nullptr;
	}

	if (m_pResultBord){
		m_pResultBord->mFinalize();
		m_pResultBord.release();
		m_pResultBord = nullptr;
	}

	m_nowDay = GameManager::eDay::eNull;
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
	if (m_gameState == eState::eExit){
		// 終了ならタイトルに戻る
		ChangeScene(SceneTitle::Name, LoadState::eUse, LoadWaitState::eUnuse);
		return true;
	}

	bool result = false;
	// 時刻の取得
	m_dayTime += GameClock::GetDeltaTime();

	if (m_dayTime > kDayTime){
		m_gameState = eState::eResult;
	}

	//m_pActionBoard->mUpdate(kCommandTimeScale);
	
	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	// それぞれのモードの更新処理
	m_pMode->mMainUpdate(m_pOrderList->mGetActionCommand(), kDefaultScaleTime, m_dayTime);
	
	m_pActionBoard->mUpdate(kCommandTimeScale);
	m_pOrderList->mUpdate(kDefaultScaleTime);

	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pMode->mMainRender(shaderHash);
	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	m_pActionBoard->mRender(shaderHash["color"].get());

	m_pOrderList->mRender(shaderHash["color"].get());

	mShowResult(m_nowDay, shaderHash["color"].get(), shaderHash["color"].get());

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


std::shared_ptr<Mode> SceneGame::mReturnMode(GameManager::eGameMode mode){
	switch (mode)
	{
	case GameManager::eGameMode::eSurvaival:
		m_nowDay = GameManager::eDay::e1day;
		mRegisterDay();
		Debug::mPrint("サバイバルモード");

		return std::make_shared<ModeSurvival>();

	case GameManager::eGameMode::ePractice:
		Debug::mPrint("プラクティスモード");
		return std::make_shared<ModePractice>();

	case GameManager::eGameMode::eNull:
		assert(!"不正な値を検出しました :Part SG");
		return nullptr;
	}
	return nullptr;
}

/*
	リザルトの表示用
*/
void SceneGame::mShowResult(GameManager::eDay nowDay, ShaderBase* defaultShader,ShaderBase* bularShader){
	if (m_gameState != eState::eResult)return;

	m_pResultBord->mSetupData(m_resultData, nowDay); // 内部的には一回しか呼ばれない

	auto mouse = GameController::GetMouse().GetMousePosition();
	bool click = GameController::GetMouse().IsLeftButtonTrigger();
	auto state = m_pResultBord->mUpdate(mouse, click);

	if (state == ResultBord::eClickState::eExit){
		m_gameState = eState::eExit;
		return;
	}
	else if (state == ResultBord::eClickState::eNextDay){
		// 次の日に進む処理
		auto prevDay = m_nowDay;
		m_pMode->mPrevDayFinalize(prevDay);
		m_nowDay = m_dayHash[prevDay];
		m_gameState = eState::eFadeIn;
		m_dayTime = NULL;
	}
	m_pResultBord->mRender(defaultShader, bularShader);

	return;
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
			// 次の日に進むにあたっての初期化処理
			m_pMode->mNextDayInitialize(m_nowDay);
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

//
void SceneGame::mRegisterDay(){
	mRegisterDayHash(GameManager::eDay::e1day, GameManager::eDay::e2day);
	mRegisterDayHash(GameManager::eDay::e2day, GameManager::eDay::e3day);
	mRegisterDayHash(GameManager::eDay::e3day, GameManager::eDay::e4day);
	mRegisterDayHash(GameManager::eDay::e4day, GameManager::eDay::e5day);
	mRegisterDayHash(GameManager::eDay::e5day, GameManager::eDay::e6day);
	mRegisterDayHash(GameManager::eDay::e6day, GameManager::eDay::e7day);
	mRegisterDayHash(GameManager::eDay::e7day, GameManager::eDay::eLastDay);
	mRegisterDayHash(GameManager::eDay::eLastDay, GameManager::eDay::eNull);

	return;
}

void SceneGame::mRegisterDayHash(GameManager::eDay key, GameManager::eDay value){
	if (m_dayHash.find(key) != m_dayHash.end())return;

	m_dayHash.insert(std::make_pair(key, value));

	return;
}