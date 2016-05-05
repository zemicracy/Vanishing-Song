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
	const float kScaleTime = 1.0f; 
	const bool kError = false;
	const float kDayTime = 180.0f; // 一日の時間
}

SceneGame::SceneGame():
GameScene(Name, GetManager()) //Sceneごとの名前を設定
{

	m_day = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{

	m_day = GameManager::eDay::eNull;
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
	mode = GameManager::eGameMode::eSurvaival;

	
	m_pResultBord = std::make_unique<ResultBord>();
	m_pResultBord->mInitialize(mode);
	
	// 現在のモードの取得
	m_pMode = mReturnMode(mode);

	// スキルの取得
	auto skill = Singleton<GameManager>::GetInstance().mSkillType();
	skill = GameManager::eSkillType::eExAttack;

	result = m_pMode->mInitialize(skill,m_day);
	if (!result){
		Debug::mErrorPrint("モードの初期化失敗", __FILE__, Debug::eState::eWindow);
		return false;
	}
	// ゲームの状態を登録
	m_gameState = eState::eRun;

	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){
	
	if (m_pMode){
		m_pMode->mFinalize();
		m_pMode.reset();
		m_pMode = nullptr;
	}
	
	if (m_pResultBord){
		m_pResultBord->mFinalize();
		m_pResultBord.release();
		m_pResultBord = nullptr;
	}

	m_day = GameManager::eDay::eNull;
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

	// デバッグ用
	if (GameController::GetKey().KeyDownTrigger('T')){
		m_gameState = eState::eResult;
	}

	if (m_gameState == eState::eExit){
		// 終了ならタイトルに戻る
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return true;
	}

	if (m_gameState == eState::eResult)return true;

	// 時刻の取得
	m_dayTime += (float)GameClock::GetDeltaTime();
	
	// 最後の日以外は制限時間になったらリザルト画面に行く
	if (m_dayTime > kDayTime && m_day != GameManager::eDay::eLastDay){
		m_gameState = eState::eResult;
	}

	bool result = false;

	// それぞれのモードの更新処理
	m_pMode->mMainUpdate(kScaleTime, m_dayTime);
	
	// 状態の更新
	auto state = m_pMode->mGetState();

	// 特にないなら何もしない
	if (state == Mode::eState::eNull)return true;

	switch (state)
	{
	case Mode::eState::eClear:
		m_gameState = eState::eResult;
		break;
	case Mode::eState::eGameOver:
	//	m_gameState = eState::eGameOver;
		break;
	case Mode::eState::eNextDay:
		m_gameState = eState::eResult;
		break;

	default:
		break;
	}
	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pMode->mMainRender(shaderHash);
	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	m_pMode->mMainUIRender(shaderHash);

	mShowResult(m_day, shaderHash["texture"].get(), shaderHash["color"].get());

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
		m_day = GameManager::eDay::e1day;
		mRegisterDay();
		Debug::mPrint("サバイバルモード");

		return std::make_shared<ModeSurvival>();

	case GameManager::eGameMode::ePractice:
		Debug::mPrint("プラクティスモード");
		return std::make_shared<ModePractice>();

	case GameManager::eGameMode::eNull:
	default:
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

	m_pResultBord->mSetupData(m_pMode->mGetResultData(), nowDay); // 内部的には一回しか呼ばれない

	auto mouse = GameController::GetMouse().GetMousePosition();
	bool click = GameController::GetMouse().IsLeftButtonTrigger();
	auto state = m_pResultBord->mUpdate(mouse, click);

	if (state == ResultBord::eClickState::eExit){
		m_gameState = eState::eExit;
		return;
	}
	else if (state == ResultBord::eClickState::eNextDay){
		// 次の日に進む処理
		auto prevDay = m_day;
		m_pMode->mPrevDayFinalize(prevDay);
		m_day = m_dayHash[prevDay];
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
			m_pMode->mNextDayInitialize(m_day);
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