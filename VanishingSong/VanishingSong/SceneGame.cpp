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
	m_pFieldPlayer->mInitialize(Singleton<ResourceManager>::GetInstance().mGetPlayerHash(eMusical::eBlue),Vector3(0,22.2,0));

	auto view = m_pFieldPlayer->mGetView();
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);
	

	m_pFieldEnemy = std::make_shared<FieldEnemyManager>();
	m_pFieldEnemy->mInitilize(view);

	m_pCollideManager = std::make_unique<CollideManager>(m_pFieldPlayer, m_pFieldArea,m_pFieldEnemy);

	m_pMessageManager = std::make_shared<MessageManager>();

	int i = 0;
	eMusical musical[3] = { eMusical::eGreen, eMusical::eRed, eMusical::eYellow };
	for (auto& index : m_pCage){
		const auto hoge = m_pFieldEnemy->mEnemyGet(i)->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation;
		index = std::make_shared<Cage>(Singleton<ResourceManager>::GetInstance().mGetPlayerHash(musical[i]), Vector3(hoge._x+20, 22.2f, hoge._z), view);
		i += 1;
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
	return true;
}

// 解放処理
// 全ての解放
void SceneGame::Finalize(){
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
bool fuga = false;
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
		m_gameState = eState::eBattle;
	}

	if (m_gameState == eState::eExit){
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return true;
	}
	else if (m_gameState == eState::eBattle){
		ChangeScene(SceneBattle::Name, LoadState::eUse);
		return true;
	}
	m_pCollideManager->mUpdate();
	auto collideInfo = m_pCollideManager->GetMassageInfo();
	const bool isPress = GameController::GetJoypad().ButtonPress(eJoyButton::eB);
	m_pMessageManager->mUpdate(collideInfo,isPress);

	m_pFieldEnemy->mUpdater();

	m_pFieldArea->mUpdate(kScaleTime);
	m_pFieldPlayer->mUpdate(kScaleTime, m_pMessageManager->mIsView());

	if (GameController::GetJoypad().ButtonPress(eJoyButton::eX)){
		fuga = !fuga;
	}

	if (fuga){
		m_pPaticle->mUpdate(kScaleTime);
	}

	for (auto& index : m_pCage){
		index->mUpdate(kScaleTime);
	}
	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	m_pFieldPlayer->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pFieldArea->mRender(shaderHash["texture"].get(), shaderHash["color"].get());

	m_pFieldEnemy->mRender(shaderHash["texture"].get(), shaderHash["color"].get());

	for (auto& index : m_pCage){
		index->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	}
	m_pPaticle->mRender(shaderHash["texture"].get());

	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pMessageManager->mRender(shaderHash["color"].get());
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