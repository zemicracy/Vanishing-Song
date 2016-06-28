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
#include "PlayDataManager.h"
#include "SceneEnd.h"
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


	// シーンの登録
	RegisterScene(new SceneTitle());
	RegisterScene(new SceneBattle());
	RegisterScene(new SceneTutorial());
	RegisterScene(new SceneEnd());

	//// フェードイン・アウトを行う
	m_pFieldPlayer = std::make_shared<FieldPlayer>();
	m_pFieldPlayer->mInitialize(ResourceManager::mGetInstance().mGetPlayerHash(eMusical::eBlue), GameManager::mGetInstance().mGetPlayerTransform());

	auto view = m_pFieldPlayer->mGetView();

	m_pFieldEnemy = std::make_shared<FieldEnemyManager>();
	m_pFieldEnemy->mInitilize(view.get());

	m_pCageManager = std::make_shared<CageManager>();
	m_pCageManager->mInitialize(m_pFieldEnemy.get(), view.get());

	m_pMessageManager = std::make_shared<MessageManager>(m_pFieldEnemy, view.get());

	m_config.mIntialize(SceneGame::Name);

	const bool isTutorial = (GameManager::mGetInstance().mFieldState() == GameManager::eFieldState::eTutorial)
		|| (GameManager::mGetInstance().mFieldState() == GameManager::eFieldState::eTutorialEnd);
	m_pTutorialEnemy = std::make_shared<TutorialEnemy>();
	m_pTutorialEnemy->mInitalize(isTutorial, ResourceManager::mGetInstance().mGetEnemyHash(eMusical::eBlue));
	if (isTutorial){
		m_gameState = eState::eTutorial;
	}
	else{
		// ゲームの状態を登録
		m_gameState = eState::eRun;
		m_pFieldEnemy->mResetEnemysTransform();
		if (GameManager::mGetInstance().mPrevEnemy().second._rotation._y != 0){
			const int number = GameManager::mGetInstance().mPrevEnemy().first;
			Transform trans = GameManager::mGetInstance().mPrevEnemy().second;
			m_pFieldEnemy->mEnemyGet(number)->mGetProperty()._pEnemy->property._transform._rotation = trans._rotation;
		}
	}

	mInitializeBGM();

	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize("Model\\Field\\game_tex");

	m_pRhythm = std::make_shared<RhythmManager>();
	if (!m_pBGMArray.empty()){
		if (m_pBGMArray.at(0)){
			m_pRhythm->mInitializeRhythm(m_pBGMArray.at(0).get(), 128);
			m_pFieldArea->mSetRhythm(m_pRhythm.get());
		}
		else{
			m_pFieldArea->mInitialize("Model\\Field\\game_tex");
			m_pFieldArea->mSetRhythm(nullptr);
		}
	}
	else{
		m_pFieldArea->mInitialize("Model\\Field\\game_tex");
		m_pFieldArea->mSetRhythm(nullptr);
	}
	m_pFieldArea->mSetCamera(view.get());


	m_pCollideManager = std::make_unique<CollideManager>(m_pFieldPlayer, m_pFieldArea, m_pFieldEnemy, m_pCageManager);

	m_isTransitionEnd = false;
	m_isFade = false;
	m_isFade2 = false;

	mInitializeFieldNote();

	// とりあえず一回呼んでおく
	mTutorial(false,false);
	mRun(false,std::make_pair(false,false));
	
	_heapmin();
	return true;
}

void SceneGame::mInitializeBGM(){
	const float volume = GameManager::mGetInstance().mGetVolume();
	m_configSE = std::make_shared<GameSound>();
	m_configSE->Load("Sound\\Title\\decision.wav");
	m_configSE->SetValume(volume);
		for (auto &itr : ResourceManager::mGetInstance().mGetBGMPath()){
			m_pBGMArray.push_back(std::make_shared<GameSound>());
			m_pBGMArray.back()->Load(itr.second.c_str());
			m_pBGMArray.back()->SetValume(volume);
			m_prevVolume = volume;
		}
		for (auto &itr : m_pBGMArray){
			itr->PlayToLoop();
		}
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
		if (!itr)continue;
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

	if (m_pTutorialEnemy){
		m_pTutorialEnemy.reset();
		m_pTutorialEnemy = nullptr;
	}

	if (m_pRhythm){
		m_pRhythm->mFinalize();
		m_pRhythm.reset();
		m_pRhythm = nullptr;
	}
	m_gameState = eState::eNull;
	_heapmin();
	return;
}

// 更新処理

bool SceneGame::Updater(){

	const bool isReturn = GameController::GetKey().KeyDownTrigger(VK_SPACE) || GameController::GetJoypad().ButtonPress(eJoyButton::eB);
	const bool isConfigButton = GameController::GetKey().KeyDownTrigger(VK_ESCAPE) || GameController::GetJoypad().ButtonPress(eJoyButton::eBack);
	std::pair<bool, bool> UpOrDown;
	UpOrDown.first = GameController::GetKey().KeyDownTrigger('W') || GameController::GetJoypad().ButtonPress(eJoyButton::eUp);
	UpOrDown.second = GameController::GetKey().KeyDownTrigger('S') || GameController::GetJoypad().ButtonPress(eJoyButton::eDown);

	std::pair<bool, bool> RightOrLeft;
	RightOrLeft.first = GameController::GetKey().KeyDownTrigger('D') || GameController::GetJoypad().ButtonPress(eJoyButton::eRight);
	RightOrLeft.second = GameController::GetKey().KeyDownTrigger('A') || GameController::GetJoypad().ButtonPress(eJoyButton::eLeft);
	if (isConfigButton){
		m_configSE->Stop();
		m_configSE->PlayToOneTime();
	}
	
	if (m_config.mUpdate(isConfigButton, isReturn, UpOrDown, RightOrLeft)){
		const float volume = GameManager::mGetInstance().mGetVolume();

		if (m_config.mGetIsBackToTitle()){
			m_gameState = eState::eExit;
			ChangeScene(SceneTitle::Name, LoadState::eUse);

			return true;
		}

		if (m_prevVolume != volume){
			for (auto& index : m_pBGMArray){
				index->SetValume(volume);
			}
			m_prevVolume = volume;
		}
		return true;
	}

	

	mTutorial(isReturn, (RightOrLeft.first || RightOrLeft.second));
	mRun(isReturn, RightOrLeft);
	return true;
}

// チュートリアル用
void SceneGame::mTutorial(bool isReturn, bool isSelect){
	if (m_gameState != eState::eTutorial)return;

	if (m_isFade){
		if (!GameManager::mGetInstance().mfadeManager().In(1)){
			return;
		}
		m_isFade = false;
		m_isFade2 = true;
		m_pFieldEnemy->mResetEnemysTransform();
		mInitializeFieldNote();
		return;
	}

	if (m_isFade2){
		if (!GameManager::mGetInstance().mfadeManager().Out(1)){
			return;
		}
		m_gameState = eState::eRun;
		m_isFade2 = false;
		return;
	}
	
	m_pFieldArea->mUpdate(kScaleTime);
	m_pFieldPlayer->mUpdate(kScaleTime, true);
	m_pCageManager->mUpdate(kScaleTime, m_pFieldPlayer->mGetTransform()._translation,false);
	if (GameManager::mGetInstance().mFieldState() == GameManager::eFieldState::eTutorial){
		if (m_pTutorialEnemy->mGetSelectType()== TutorialEnemy::eSelect::eYes){
			m_gameState = eState::eBattle;
			m_pTutorialEnemy->mIsEnd(true);
			GameManager::mGetInstance().mPushUsePlayer(eMusical::eBlue);
			GameManager::mGetInstance().mSetPlayerTransform(m_pFieldPlayer->mGetTransform());
			GameManager::mGetInstance().mBattleDataFile(m_pTutorialEnemy->mGetDataPath());
			ChangeScene(SceneTutorial::Name, LoadState::eUse);
			return;
		}
		else if (m_pTutorialEnemy->mGetSelectType() == TutorialEnemy::eSelect::eNo){
			if (m_pTutorialEnemy->mGetMessageEnd()){

				GameManager::mGetInstance().mPushUsePlayer(eMusical::eBlue);
				ResourceManager::mGetInstance().mSetBGMPath(eMusical::eBlue) = "Sound\\BGM\\field1.wav";
				mInitializeBGM();
				m_pRhythm->mInitializeRhythm(m_pBGMArray.at(0).get(), 128);
				m_pFieldArea->mSetRhythm(m_pRhythm.get());
				GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eFirstStage);
				m_isFade = true;
				m_pTutorialEnemy->mIsEnd(true);
			}
		}

		m_pFieldEnemy->mUpdater();
		m_pTutorialEnemy->mUpdate(false, isSelect, isReturn);
	}
	else if (GameManager::mGetInstance().mFieldState() == GameManager::eFieldState::eTutorialEnd){
		// チュートリアル終了後
		if (m_pTutorialEnemy->mGetMessageEnd()){
			GameManager::mGetInstance().mPushUsePlayer(eMusical::eBlue);
			GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eFirstStage);
			m_isFade = true;
			m_pTutorialEnemy->mIsEnd(true);
			return;
		}
		m_pFieldEnemy->mUpdater();
		m_pTutorialEnemy->mUpdate(true, false, isReturn);
		
	}
}

//
void SceneGame::mRun(const bool isReturn, const std::pair<bool, bool> RightOrLeft){
	if (m_gameState != eState::eRun)return;
	
	m_pCollideManager->mUpdate();

	// メッセージの更新処理
	bool changeBattle = mMessageUpdate(isReturn, RightOrLeft.first || RightOrLeft.second);
	if (changeBattle){
		// 戦闘に行く処理
		// 戦闘に行く前に設定する奴もここでする
		m_gameState = eState::eBattle;
		ChangeScene(SceneBattle::Name, LoadState::eUse);
		return;
	}
	
	m_pFieldEnemy->mUpdater();
	m_pFieldArea->mUpdate(kScaleTime);
	const int fieldNumber = m_pFieldPlayer->mGetFieldNumber();
	m_pFieldPlayer->mUpdate(kScaleTime, m_pMessageManager->mIsView()||m_pCageManager->mGetIsMessageRun(fieldNumber));
	m_fieldNote.mUpdate();
}

//
void SceneGame::Render(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	if (m_pTutorialEnemy){
		m_pTutorialEnemy->mRender(shaderHash["texture"].get());
	}
	m_pFieldPlayer->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pFieldArea->mRender(shaderHash["texture"].get(), shaderHash["transparent"].get());
	m_pMessageManager->m3DRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_fieldNote.mRender(shaderHash["transparent"].get());
	m_pCageManager->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	m_pFieldEnemy->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	return;
}

//
void SceneGame::UIRender(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	m_pMessageManager->m2DRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	m_pCageManager->mUIRender(shaderHash["transparent"].get());
	if (m_isTransitionEnd){
		m_pTutorialEnemy->mUIRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	}
	m_config.mUIRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	GameManager::mGetInstance().mfadeManager().mRender(shaderHash["color"].get());
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
	m_isTransitionEnd = true;
	return kTransitionEnd;
}

// メッセージの更新処理
bool SceneGame::mMessageUpdate(const bool isReturn, const bool isSelect){

	auto collideInfo = m_pCollideManager->GetMassageInfo();
	const bool isPress = isReturn;
	const bool selectButton = isSelect;
	const Vector3 playerPosition = m_pFieldPlayer->mGetBodyColldier()->property._transform._translation;
	const Vector3 enemyPosition = m_pFieldEnemy->mEnemyGet(collideInfo.first)->mGetProperty()._pCollider->property._transform._translation;

	const int canStageNumber = GameManager::mGetInstance().mGetCanStage();
	m_pMessageManager->mUpdate(collideInfo, isPress, selectButton,playerPosition,enemyPosition,canStageNumber);

	if (m_pMessageManager->mGetIsChangeScene()){
		GameManager::mGetInstance().mSetPlayerTransform(m_pFieldPlayer->mGetTransform());
		GameManager::mGetInstance().mBattleDataFile(m_pFieldEnemy->mEnemyGet(collideInfo.first)->mGetBattleDataPath());
		GameManager::mGetInstance().mPrevEnemy(collideInfo.first, m_pFieldEnemy->mEnemyGet(collideInfo.first)->mGetProperty()._pEnemy->property._transform);
		return true;
	}

	m_pCageManager->mUpdate(kScaleTime, m_pFieldPlayer->mGetTransform()._translation, isPress);
	return false;
}

//
void SceneGame::mInitializeFieldNote(){
	for (auto& usePlayer : GameManager::mGetInstance().mGetUsePlayer()){
		m_fieldNote.mInitialize(usePlayer.second, m_pFieldPlayer->mGetView().get(), Vector3(-400.0f, 0.f, -400.0f), Vector3(400.0f, 0.0f, 400.0f));
	}
}