#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
using namespace aetherClass;
namespace{
	const float kDefaultScaleTime = 100.0f; 
	const float kCommandTimeScale = 1.0f;
	const bool kError = false;
}
const std::string SceneGame::Name = "Game";

SceneGame::SceneGame():
GameScene(Name, GetManager()) //Sceneごとの名前を設定
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}

bool SceneGame::Initialize(){

	bool result = false;

	result = mInitalizeShader();
	if (!result){
		Debug::mErrorPrint("シェーダー初期化失敗", __FILE__, __LINE__);
		return false;
	}

	
	// プレイヤーの初期化
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->mInitialize();

	auto view = m_pPlayer->mGetView();

	m_penemyGround = std::make_shared<EnemyGround>();
	m_penemyGround->mInitialize(view);

	// アクションコマンドの初期化
	m_pActionBoard = std::make_unique<ActionBoard>();
	m_pActionBoard->mInitialize(GameManager::eSkillType::eExAttack);

	// オーダーリストの初期化
	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize();

	// ステージオブジェクト
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	// フェードイン・アウトを行う
	m_pFadeObject = std::make_unique<FadeManager>();
	// ゲームモードの取得
	// このシーンに来ている時点で、サバイバルか、プラクティスが選択されている
	m_nowMode = Singleton<GameManager>::GetInstance().mGameMode(); 
	m_nowMode = GameManager::eGameMode::eSurvaival;
	mInitializeMode(m_nowMode);
	m_dayTime = 0.0f;
	
	m_pResultBord = std::make_unique<ResultBord>();
	m_pResultBord->mInitialize(m_nowMode);

	RegisterScene(new SceneTitle());

	m_pCollideManager = std::make_unique<CollideManager>();

	m_pCollideManager->mInitialize(m_pPlayer, nullptr, m_pFieldArea);
	// ゲームの状態を登録
	m_gameState = eState::eRun;

	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){
	if (m_pResultBord){
		m_pResultBord->mFinalize();
		m_pResultBord.release();
		m_pResultBord = nullptr;
	}

	if (m_pActionBoard){
		m_pActionBoard.release();
		m_pActionBoard = nullptr;
	}

	if (m_pOrderList){
		m_pOrderList.release();
		m_pOrderList = nullptr;
	}

	if (m_pFieldArea){
		m_pFieldArea.reset();
		m_pFieldArea = nullptr;
	}

	if (m_pPlayer){
		m_pPlayer->mFinalize();
		m_pPlayer.reset();
		m_pPlayer = nullptr;
	}

	if (m_penemyGround){
		m_penemyGround->mFinalize();
		m_penemyGround.reset();
		m_penemyGround = nullptr;
	}
	m_nowMode = GameManager::eGameMode::eNull;
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
	}

	bool result = false;
	// 時刻の取得
	m_dayTime += GameClock::GetDeltaTime();
	// サバイバルモード時の更新処理
	result = this->mSurvivalMainUpdate(kDefaultScaleTime, m_dayTime);
	if (!result){
		return kError;
	}

	// ぷラクティモード時の更新処理
	result = this->mPracticeMainUpdate(kDefaultScaleTime, m_dayTime);
	if (!result){
		return kError;
	}

	m_pCollideManager->mUpdate();
	return true;
}

void SceneGame::Render(){
	this->mSurvivalRender();
	this->mPracticeRender();
	
	return;
}

void SceneGame::UIRender(){
	this->mSurvivalUIRender();
	this->mPracticeUIRender();
	mShowResult(m_nowDay, m_pixelShader.get(), m_pixelShader.get());

	if (m_gameState == eState::eFadeIn || m_gameState == eState::eFadeOut){
		m_pFadeObject->mRedner(m_pixelShader.get());
	}
	return;
}

bool SceneGame::TransitionIn(){

	return kTransitionEnd;
}

bool SceneGame::TransitionOut(){

	return kTransitionEnd;
}

bool SceneGame::mInitalizeShader(){
	// シェーダーの詳細情報の設定
	ShaderDesc textureDesc;
	bool result = false;
	textureDesc._vertex._entryName = "vs_main";
	textureDesc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	textureDesc._pixel._entryName = "ps_main";
	textureDesc._pixel._srcFile = L"Shader/BasicColor.hlsl";

	// ピクセルシェーダーの作成
	m_pixelShader = std::make_shared<PixelShader>();
	result = m_pixelShader->Initialize(textureDesc, ShaderType::eVertex | ShaderType::ePixel);
	if (!result){
		return false;
	}

	return true;
}

/*
	それぞれのモードの独自の初期化
*/
bool SceneGame::mInitializeMode(GameManager::eGameMode mode){
	switch (mode)
	{
	case GameManager::eGameMode::eSurvaival:
		m_nowDay = GameManager::eDay::e1day;
		mRegisterDay();
		Debug::mPrint("サバイバルモード");

		break;
	case GameManager::eGameMode::ePractice:
		Debug::mPrint("プラクティスモード");
		break;
	case GameManager::eGameMode::eNull:
		assert(!"不正な値を検出しました :Part SG");
		return false;
	}
	return true;
}


/*
	サバイバルモード時の更新処理
*/
bool SceneGame::mSurvivalMainUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::eSurvaival || m_gameState != eState::eRun)return true;
	if (GameController::GetKey().KeyDownTrigger(VK_RETURN)){
		m_gameState = eState::eResult;
	}

	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	m_pPlayer->mCommand(m_pOrderList->mGetActionCommand(), kDefaultScaleTime);
	m_pPlayer->mUpdate(kDefaultScaleTime);

	m_pActionBoard->mUpdate(kCommandTimeScale);
	m_pOrderList->mUpdate(kDefaultScaleTime);

	return true;
}

/*
	サバイバルモード時の描画処理
*/
void SceneGame::mSurvivalRender(){
	if (m_nowMode != GameManager::eGameMode::eSurvaival)return;
	auto view = m_pPlayer->mGetView();
	view->Render();
	m_pPlayer->mRender(m_pixelShader.get(), m_pixelShader.get());

	m_penemyGround->mRender(m_pixelShader.get(),m_pixelShader.get());
	m_pFieldArea->mRender(m_pixelShader.get());

	
}

//
void SceneGame::mSurvivalUIRender(){
	if (m_nowMode != GameManager::eGameMode::eSurvaival)return;

	m_pActionBoard->mRender(m_pixelShader.get());
	m_pOrderList->mRender(m_pixelShader.get());
	
	
}


/*
	プラクティス以下略
*/
bool SceneGame::mPracticeMainUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::ePractice || m_gameState != eState::eRun)return true;

}

/*
 プラクry
*/
void SceneGame::mPracticeRender(){
	if (m_nowMode != GameManager::eGameMode::ePractice)return;
}

//
void SceneGame::mPracticeUIRender(){
	if (m_nowMode != GameManager::eGameMode::ePractice)return;
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
		m_nowDay = m_dayHash[prevDay];
		m_gameState = eState::eFadeIn;
	}
	m_pResultBord->mRender(defaultShader, bularShader);
	return;
}

void SceneGame::mRegisterDayHash(GameManager::eDay key, GameManager::eDay value){
	if (m_dayHash.find(key) != m_dayHash.end())return;

	m_dayHash.insert(std::make_pair(key, value));

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