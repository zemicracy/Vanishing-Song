#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
#include "SceneBattle.h"
#include "SceneTutorial.h"
#include "ResourceManager.h"
#include "Debug.h"
using namespace aetherClass;

const std::string SceneGame::Name = "Game";

namespace{
	const float kScaleTime = 1.0f; 
	const bool kError = false;
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
	_heapmin();
	Finalize();
	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eGreen, "Model\\Player", "Model\\Player\\green");
	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eYellow, "Model\\Player", "Model\\Player\\yellow");

	// シーンの登録
	RegisterScene(new SceneTitle());
	RegisterScene(new SceneBattle());
	RegisterScene(new SceneTutorial());

	//// フェードイン・アウトを行う
	m_pFieldPlayer = std::make_shared<FieldPlayer>();
	m_pFieldPlayer->mInitialize(ResourceManager::mGetInstance().mGetPlayerHash(eMusical::eBlue), GameManager::mGetInstance().mGetPlayerTransform());

	auto view = m_pFieldPlayer->mGetView();
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);


	m_pFieldEnemy = std::make_shared<FieldEnemyManager>();
	m_pFieldEnemy->mInitilize(view);

	m_pCollideManager = std::make_unique<CollideManager>(m_pFieldPlayer, m_pFieldArea, m_pFieldEnemy);

	m_pMessageManager = std::make_shared<MessageManager>(m_pFieldEnemy, view);

	m_pCageManager = std::make_shared<CageManager>();
	m_pCageManager->mInitialize(m_pFieldEnemy.get(), view);

	{
		for (auto &itr : ResourceManager::mGetInstance().mGetBGMPath()){
			m_pBGMArray.push_back(std::make_shared<GameSound>());
			m_pBGMArray.back()->Load(itr.second.c_str());
			m_pBGMArray.back()->SetValume(0);
		}
		for (auto &itr : m_pBGMArray){
			itr->PlayToLoop();
		}
	}

	// とりあえず一回呼んでおく
	mRun();
	
	
	const bool isTutorial = (GameManager::mGetInstance().mFieldState() == GameManager::eFieldState::eTutorial)
		|| (GameManager::mGetInstance().mFieldState() == GameManager::eFieldState::eTutorialEnd);
	m_pTutorialEnemy = std::make_shared<TutorialEnemy>();
	m_pTutorialEnemy->mInitalize(isTutorial);
	if (isTutorial){
		m_gameState = eState::eTutorial;
	}
	else{
		// ゲームの状態を登録
		m_gameState = eState::eRun;
	}
	_heapmin();
	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){
	_heapmin();
	if (m_pCageManager){
		m_pCageManager.reset();
		m_pCageManager = nullptr;
	}

	for (auto &itr : m_pBGMArray){
		itr->Stop();
		itr.reset();
	}
	if (m_pMessageManager){
		m_pMessageManager.reset();
		m_pMessageManager = nullptr;
	}
	if (m_pCollideManager){
		m_pCollideManager.reset();
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

	if (m_pFieldEnemy){
		m_pFieldEnemy.reset();
		m_pFieldEnemy = nullptr;
	}
	
	m_gameState = eState::eNull;
	_heapmin();
	return;
}

// 更新処理

bool SceneGame::Updater(){

	mTutorial();
	mRun();
	
	return true;
}

// チュートリアル用
void SceneGame::mTutorial(){
	if (m_gameState != eState::eTutorial)return;
	bool button = GameController::GetKey().KeyDownTrigger(VK_SPACE);
	if (GameManager::mGetInstance().mFieldState() == GameManager::eFieldState::eTutorial){
		if (button){
			m_gameState = eState::eBattle;
			GameManager::mGetInstance().mSetPlayerTransform(m_pFieldPlayer->mGetTransform());
			GameManager::mGetInstance().mBattleDataFile(m_pTutorialEnemy->mGetDataPath());
			ChangeScene(SceneTutorial::Name, LoadState::eUse);
			return;
		}
	}
	else{
		// チュートリアル終了後
		if (button){
			GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eFirstStage);
			m_gameState = eState::eRun;
			return;
		}
	}
}

//
void SceneGame::mRun(){
	if (m_gameState != eState::eRun)return;
	//// タイトルに戻る
	if (GameController::GetKey().KeyDownTrigger(VK_ESCAPE)){
		m_gameState = eState::eExit;
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return;
	}

	m_pCollideManager->mUpdate();

	// メッセージの更新処理
	bool changeBattle = mMessageUpdate();
	if (changeBattle){
		// 戦闘に行く処理
		// 戦闘に行く前に設定する奴もここでする
		m_gameState = eState::eBattle;

		ChangeScene(SceneBattle::Name, LoadState::eUse);
		return;
	}

	m_pFieldEnemy->mUpdater();
	m_pFieldArea->mUpdate(kScaleTime);
	m_pFieldPlayer->mUpdate(kScaleTime, m_pMessageManager->mIsView());
	m_pCageManager->mUpdate(kScaleTime, m_pFieldPlayer->mGetTransform()._translation);
}

//
void SceneGame::Render(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	
	m_pFieldPlayer->mRender(shaderHash["texture"].get(), shaderHash["color"].get());

	m_pFieldEnemy->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pCageManager->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pFieldArea->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pMessageManager->m3DRender(shaderHash["texture"].get(), shaderHash["color"].get());
	
	
	return;
}

//
void SceneGame::UIRender(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	m_pMessageManager->m2DRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	GameManager::mGetInstance().mfadeManager().mRedner(shaderHash["color"].get());
	return;
}

//
bool SceneGame::TransitionIn(){
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		return kTransitionning;
	}

	return kTransitionEnd;
}

//
bool SceneGame::TransitionOut(){
	if (!GameManager::mGetInstance().mfadeManager().Out(1)){
		return kTransitionning;
	}

	return kTransitionEnd;
}

// メッセージの更新処理
bool SceneGame::mMessageUpdate(){

	auto collideInfo = m_pCollideManager->GetMassageInfo();
	const bool isPress = GameController::GetJoypad().ButtonRelease(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger(VK_SPACE);
	const bool selectButton = GameController::GetJoypad().ButtonPress(eJoyButton::eLeft) || GameController::GetJoypad().ButtonPress(eJoyButton::eRight) ||
		GameController::GetKey().KeyDownTrigger('A') || GameController::GetKey().KeyDownTrigger('D');

	const Vector3 playerPosition = m_pFieldPlayer->mGetBodyColldier()->property._transform._translation;
	const Vector3 enemyPosition = m_pFieldEnemy->mEnemyGet(collideInfo.first)->mGetProperty()._pCollider->property._transform._translation;

	const int canStageNumber = GameManager::mGetInstance().mGetCanStage();
	m_pMessageManager->mUpdate(collideInfo, isPress, selectButton,playerPosition,enemyPosition,canStageNumber);

	if (m_pMessageManager->mGetIsChangeScene()){
		GameManager::mGetInstance().mSetPlayerTransform(m_pFieldPlayer->mGetTransform());
		GameManager::mGetInstance().mBattleDataFile(m_pFieldEnemy->mEnemyGet(collideInfo.first)->mGetBattleDataPath());

		return true;
	}
	return false;
}

