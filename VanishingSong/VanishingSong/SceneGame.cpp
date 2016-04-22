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
	const float kDefaultScaleTime = 1.0f;
}
const std::string SceneGame::Name = "Game";
SceneGame::SceneGame():
GameScene(Name, GetManager()) //Sceneごとの名前を設定
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_dayTime = NULL;
}


SceneGame::~SceneGame()
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
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
	m_pPlayer = std::make_unique<Player>();
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

	m_pFieldArea = std::make_unique<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	// ゲームモードの取得
	// このシーンに来ている時点で、サバイバルか、プラクティスが選択されている
	m_nowMode = Singleton<GameManager>::GetInstance().mGameMode();
	m_nowDay = GameManager::eDay::e1day;
	mInitializeMode(m_nowMode);
	m_dayTime = 0.0f;
	
	RegisterScene(new SceneTitle());
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

	if (m_pFieldArea){
		m_pFieldArea.release();
		m_pFieldArea = nullptr;
	}

	if (m_pPlayer){
		m_pPlayer->mFinalize();
		m_pPlayer.release();
		m_pPlayer = nullptr;
	}

	if (m_penemyGround){
		m_penemyGround->mFinalize();
		m_penemyGround.reset();
		m_penemyGround = nullptr;
	}
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_dayTime = NULL;
	return;
}

// 更新処理
bool SceneGame::Updater(){
	bool result = false;
	// 時刻の取得
	m_dayTime += GameClock::GetDeltaTime();
	// サバイバルモード時の更新処理
	result = this->mSurvivalUpdate(kDefaultScaleTime, m_dayTime);
	if (!result){
		return false;
	}

	// ぷラクティモード時の更新処理
	result = this->mPracticeUpdate(kDefaultScaleTime,m_dayTime);
	if (!result){
		return false;
	}
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
bool SceneGame::mSurvivalUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::eSurvaival)return true;

	m_pPlayer->mUpdate(kDefaultScaleTime);

	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	m_pPlayer->mCommand(m_pOrderList->mGetActionCommand(), kDefaultScaleTime);

	m_pActionBoard->mUpdate(kDefaultScaleTime);
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
bool SceneGame::mPracticeUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::ePractice)return true;

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
