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
	bool result = true;

	Finalize();

	// シーンの登録
	RegisterScene(new SceneTitle());
	RegisterScene(new SceneBattle());

	// フェードイン・アウトを行う
	
	m_pFieldPlayer = std::make_shared<FieldPlayer>();
	m_pFieldPlayer->mInitialize(ResourceManager::mGetInstance().mGetPlayerHash(eMusical::eBlue),Vector3(0,22.2,0));

	auto view = m_pFieldPlayer->mGetView();
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);
	

	m_pFieldEnemy = std::make_shared<FieldEnemyManager>();
	m_pFieldEnemy->mInitilize(view);

	m_pCollideManager = std::make_unique<CollideManager>(m_pFieldPlayer, m_pFieldArea,m_pFieldEnemy);

	m_pMessageManager = std::make_shared<MessageManager>(m_pFieldEnemy,view);

	int count= 0;
	eMusical musical[3] = { eMusical::eGreen, eMusical::eRed, eMusical::eYellow };
	for (auto& index : m_pCage){
		const auto hoge = m_pFieldEnemy->mEnemyGet(count)->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation;
		index = std::make_shared<Cage>(ResourceManager::mGetInstance().mGetPlayerHash(musical[count]), Vector3(hoge._x + 20, kPlayerInitializeY, hoge._z), view);
		count += 1;
	}

	// ゲームの状態を登録
	m_gameState = eState::eRun;
	m_pFieldPlayer->mSetTransform(GameManager::mGetInstance().mGetPlayerTransform());

	// ボスに勝っていたら完成の音楽を流す
	auto bossState = GameManager::mGetInstance().mBossState();
	if (bossState == GameManager::eBossState::eWin){
		ResourceManager::mGetInstance().mGetLastBGM()->PlayToLoop();
	}
	else{
		auto fieldState = GameManager::mGetInstance().mFieldState();
		if (fieldState != GameManager::eFieldState::eTutorial){
			for (auto index : GameManager::mGetInstance().mNote()){
				ResourceManager::mGetInstance().mPlayBaseBGM(index);
			}
		}
		else{
			ResourceManager::mGetInstance().mGetFirstBGM()->PlayToLoop();
		}
	}
	
	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){
	for (auto index : GameManager::mGetInstance().mGetUsePlayer()){
		ResourceManager::mGetInstance().mStopBaseBGM(index.second);
	}
	ResourceManager::mGetInstance().mGetLastBGM()->Stop();
	ResourceManager::mGetInstance().mGetFirstBGM()->Stop();

	if (!m_pCage.empty()){
		for (auto& index : m_pCage){
			if (!index)continue;
			index->mFinalize();
		}
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
		m_pFieldEnemy->mFinalize();
		m_pFieldEnemy = nullptr;
	}
	
	m_gameState = eState::eNull;
	return;
}

// 更新処理
bool SceneGame::Updater(){

	// タイトルに戻る
	if (GameController::GetKey().KeyDownTrigger(VK_ESCAPE)){
		m_gameState = eState::eExit;
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return true;
	}

	m_pCollideManager->mUpdate();

	// メッセージの更新処理
	bool changeBattle = mMessageUpdate();
	if (changeBattle){
		// 戦闘に行く処理
		// 戦闘に行く前に設定する奴もここでする
		m_gameState = eState::eBattle;

		ChangeScene(SceneBattle::Name, LoadState::eUse);
		return true;
	}
	
	m_pFieldEnemy->mUpdater();

	m_pFieldArea->mUpdate(kScaleTime);
	m_pFieldPlayer->mUpdate(kScaleTime, m_pMessageManager->mIsView());

	// 捕虜を向かせる
	for (auto& index : m_pCage){
		index->mUpdate(kScaleTime,m_pFieldPlayer->mGetBodyColldier()->property._transform._translation);
	}
	return true;
}

void SceneGame::Render(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();

	m_pFieldPlayer->mRender(shaderHash["texture"].get(), shaderHash["color"].get());

	// 捕虜の表示
	for (auto& index : m_pCage){
		index->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	}

	m_pFieldEnemy->mRender(shaderHash["texture"].get(), shaderHash["color"].get());

	m_pFieldArea->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pMessageManager->m3DRender(shaderHash["texture"].get(), shaderHash["color"].get());
	
	return;
}

void SceneGame::UIRender(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	m_pMessageManager->m2DRender(shaderHash["transparent"].get(), shaderHash["color"].get());

	return;
}

bool SceneGame::TransitionIn(){

	return kTransitionEnd;
}

bool SceneGame::TransitionOut(){

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