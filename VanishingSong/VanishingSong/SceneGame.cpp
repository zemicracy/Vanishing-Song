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
	m_pFadeObject = std::make_unique<FadeManager>();

	m_pFieldPlayer = std::make_shared<FieldPlayer>();
	m_pFieldPlayer->mInitialize(Singleton<ResourceManager>::GetInstance().mGetPlayerHash(eMusical::eBlue),Vector3(0,22.2,0));

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
		index = std::make_shared<Cage>(Singleton<ResourceManager>::GetInstance().mGetPlayerHash(musical[count]), Vector3(hoge._x + 20, 22.2f, hoge._z), view);
		count += 1;
	}

	AttackParticle::ParticleDesc particle;
	particle._size = 100;
	particle._endPoint = Vector3(0, 10, 0);
	particle._rangeMin = Vector3(-10, 0, 0);
	particle._rangeMax = Vector3(10, 0, 0);
	particle._scale = Vector3(2,2, 0);
	particle._texturePath = "Texture\\Battle\\note.png";
	m_pPaticle = std::make_shared<AttackParticle>(particle,view);

	// ゲームの状態を登録
	m_gameState = eState::eRun;
	m_pFieldPlayer->mSetTransform(Singleton<GameManager>::GetInstance().mGetPlayerTransform());

	// ボスに勝っていたら完成の音楽を流す
	auto bossState = Singleton<GameManager>::GetInstance().mFieldBossState();
	if (bossState == GameManager::eBossState::eWin){
		Singleton<ResourceManager>::GetInstance().mGetLastBGM()->PlayToLoop();
	}
	else{
		for (auto index : Singleton<GameManager>::GetInstance().mGetUsePlayer()){
			Singleton<ResourceManager>::GetInstance().mPlayBaseBGM(index.second);
		}
	}
	
	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){
	for (auto index : Singleton<GameManager>::GetInstance().mGetUsePlayer()){
		Singleton<ResourceManager>::GetInstance().mStopBaseBGM(index.second);
	}
	Singleton<ResourceManager>::GetInstance().mGetLastBGM()->Stop();
	if (m_pMessageManager){
		m_pMessageManager.reset();
		m_pMessageManager = nullptr;
	}
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
	if (m_pFieldEnemy){
		m_pFieldEnemy->mFinalize();
		m_pFieldEnemy = nullptr;
	}
	
	m_gameState = eState::eNull;
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
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	m_pFieldPlayer->mRender(shaderHash["texture"].get(), shaderHash["color"].get());

	// 捕虜の表示
	for (auto& index : m_pCage){
		index->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	}

	m_pFieldEnemy->mRender(shaderHash["texture"].get(), shaderHash["color"].get());

	m_pFieldArea->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pMessageManager->m3DRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pPaticle->mRender(shaderHash["texture"].get());

	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pMessageManager->m2DRender(shaderHash["texture"].get(), shaderHash["color"].get());
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

// メッセージの更新処理
bool SceneGame::mMessageUpdate(){
	auto collideInfo = m_pCollideManager->GetMassageInfo();
	const bool isPress = GameController::GetJoypad().ButtonRelease(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger(VK_SPACE);
	const bool selectButton = GameController::GetJoypad().ButtonPress(eJoyButton::eLeft) || GameController::GetJoypad().ButtonPress(eJoyButton::eRight) ||
		GameController::GetKey().KeyDownTrigger('A') || GameController::GetKey().KeyDownTrigger('D');
	const Vector3 playerPosition = m_pFieldPlayer->mGetBodyColldier()->property._transform._translation;
	const Vector3 enemyPosition = m_pFieldEnemy->mEnemyGet(collideInfo.first)->mGetProperty()._pCollider->property._transform._translation;

	m_pMessageManager->mUpdate(collideInfo, isPress, selectButton,playerPosition,enemyPosition);
	if (m_pMessageManager->mGetIsChangeScene()){

		Singleton<GameManager>::GetInstance().mSetPlayerTransform(m_pFieldPlayer->mGetTransform());
		Singleton<GameManager>::GetInstance().mBattleDataFile(m_pFieldEnemy->mEnemyGet(collideInfo.first)->mGetBattleDataPath());

		return true;
	}
	return false;
}