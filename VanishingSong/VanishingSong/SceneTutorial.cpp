#include "SceneTutorial.h"
#include <iostream>
#include<Skybox.h>
#include<Rectangle3D.h>
#include<PixelShader.h>
#include <GameController.h>
#include "ResourceManager.h"
#include"SceneGame.h"
#include"Cipher.h"
//debug
using namespace aetherClass;
const std::string SceneTutorial::Name = "Tutorial";
SceneTutorial::SceneTutorial() :
GameScene(Name, GetManager())
{
}

SceneTutorial::~SceneTutorial()
{
}


//interim

bool SceneTutorial::Initialize(){
	_heapmin();

	mLoadTextData();

	RegisterScene(new SceneGame());

	m_pMessage = std::make_unique<BattleMessage>();

	m_pActionBoard = std::make_shared<ActionBoard>();
	m_pActionBoard->mInitialize();


	m_pField = std::make_unique<BattleField>();
	m_pField->mInitialize(&m_view, m_rhythm.get(),false);

	m_pBattleEnemyManager = std::make_shared<BattleEnemyManager>();
	m_pBattleEnemyManager->mInitialize(&m_view, m_pField.get());

	//ウェーブ系
	m_MaxWave = m_pBattleEnemyManager->mGetWaveAllCount();
	m_waveID = 1;
	m_pBattleEnemyManager->ResetEnemyList(m_waveID - 1, &m_view);

	m_pGauge = std::make_unique<GaugeManager>();
	m_pGauge->mInitialize(m_rhythm.get());

	//hp
	m_enemyHp = &m_pBattleEnemyManager->mGetCharaStatus(m_waveID - 1);
	m_enemyHp->_maxHp  = m_enemyHp->_hp = 8;
	m_pGauge->mSetHpAll(&m_charaHp, m_enemyHp);


	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(m_beatMode, m_battleState, m_pActionBoard.get(), m_pField.get(), m_rhythm.get());

	// プレイヤーの初期化
	for (auto& index : GameManager::mGetInstance().mGetUsePlayer()){
		auto gearframe = ResourceManager::mGetInstance().mGetPlayerHash(index.second);
		gearframe->SetCamera(&m_view);
		m_players.mSetPlayer(index.second, m_pField->mGetPlayerLane(index.second), gearframe);
		m_charaHp._maxHp = m_charaHp._hp += 8;
	}

	m_pTutorial = std::make_unique<TutorialMessage>();
	m_pTutorial->mInitialize();
	m_pTutorial->mSetVisible(false);

	m_battleState = GameManager::eBattleState::eNewWave;
	m_processState = eGameState::ePreCountIn;
	m_initUpdateProcess = false;
	m_preInitProcess = false;
	m_prevWholeBeatNo = 0;
	m_textReadCnt = 1;
	

	m_particleDesc._spawnRate = 10;
	m_particleDesc._scale = 5;
	m_particleDesc._texturePath = "Texture\\OrderList\\note_a.png";
	m_particleDesc._rangeMin = Vector3(0, 0, 0);
	m_particleDesc._rangeMax = Vector3(20, 0, 20);
	m_particleDesc._startPoint = m_pField->mGetEnemyLane(eMusical::eBlue);
	m_particleDesc._endPoint = m_pField->mGetEnemyLane(eMusical::eBlue);
	m_particleDesc._endPoint._y += 100;


	m_bgmVolume = 60;
	m_inCount = 0;

	m_tutorialState = eTutorialState::eInit;
	m_isTutorialPlay = false;
	m_timeEngage = false;
	m_isEndTransition = false;
	
	m_players.mUpdate(0, eMusical::eNull);
	m_pOrderList->mUpdate();


	//チュートリアルWave１
	m_enemyVector.reserve(4);
	eMusical askey[4] = { eMusical::eBlue, eMusical::eNull, eMusical::eBlue, eMusical::eNull };
	for (int i = 0; i < 4; ++i){
			m_enemyVector.push_back(m_pActionBoard->mGetCommand(askey[i]));
	}

	//最後に行う
	m_sound->SetValume(-m_bgmVolume * 100);
	_heapmin();
	return true;
}

void SceneTutorial::Finalize(){
	_heapmin();
	if (m_pOrderList){
		m_pOrderList.reset();
	}
	if (m_pTutorial){
		m_pTutorial.reset();
	}
	if (m_sound){
		m_sound.reset();
	}
	if (m_pActionBoard){
		m_pActionBoard.reset();
	}
	if (m_pField){
		m_pField.reset();
	}
	if (m_pGauge){
		m_pGauge.reset();
	}
	if (m_pMessage){
		m_pMessage.reset();
	}
	if (m_pResult){
		m_pResult.reset();
	}
	if (m_rhythm){
		m_rhythm->mFinalize();
	}
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager.reset();
	}
	m_sound.reset();
	m_rhythm->mFinalize();
	m_rhythm.reset();

	for (int i = 0; i < 20; ++i){
		_heapmin();
	}
	return;
}
void SceneTutorial::mLoadTextData(){
	std::string file = "data\\Battle\\Tutorial";
	GameManager::mGetInstance().mBattleDataFile(file);

	Cipher cip(file);

	m_stageID = std::atoi(&cip.mGetSpriteArray("[Stage]").front().front());
	int type = std::atoi(&cip.mGetSpriteArray("[Beat]").front().front());
	m_beatMode = GameManager::eGameMode::eQuarter;

	m_stageID = 0;
	
	//サウンドのデータも読み込む
	m_sound = std::make_shared<GameSound>();
	m_sound->Load("Sound\\Battle\\tutorial100.wav");

	m_rhythm = std::make_shared<RhythmManager>();
	m_rhythm->mInitializeRhythm(m_sound, 100);
	m_rhythm->mAcquire();

	cip.mUnLoad();
}


//ここで一旦止めたりする
bool SceneTutorial::mTutorialUpdater(){
	bool result = true;
	m_view.Controller();
	//説明中じゃなければ止めない
	if (!m_isTutorialPlay){
		return result;
	}

	const bool isPress = GameController::GetJoypad().ButtonRelease(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger(VK_SPACE);
	if (isPress){
		m_textReadCnt++;
	}

	m_pTutorial->mSetVisible(true);

	switch (m_tutorialState)
	{		//次の画像が無いとFalseを返してくる
	case eTutorialState::eInit:
			result = m_pTutorial->mChangeText("Init" + std::to_string(m_textReadCnt));
			if (!result){
				m_tutorialState = eTutorialState::eFirstDemo;
			}
		break;
	case eTutorialState::eFirstDemo:
		if (m_battleState == GameManager::eBattleState::eListen){
			result = m_pTutorial->mChangeText("FirstDemoListen" + std::to_string(m_textReadCnt));
		}
		else if (m_battleState == GameManager::eBattleState::ePerform){
			result = m_pTutorial->mChangeText("FirstDemoPerform" + std::to_string(m_textReadCnt));
		}
		else if (m_battleState == GameManager::eBattleState::eBattle){
			result = m_pTutorial->mChangeText("FirstDemoBattle" + std::to_string(m_textReadCnt));
		}
		break;
	case eTutorialState::eFirstPlay:
		result = m_pTutorial->mChangeText("FirstPlay" + std::to_string(m_textReadCnt));
		break;
	case eTutorialState::eAdlibDemo:
		if (m_battleState == GameManager::eBattleState::eListen){
			result = m_pTutorial->mChangeText("AdlibDemoListen" + std::to_string(m_textReadCnt));
		}
		else if (m_battleState == GameManager::eBattleState::ePerform){
			result = m_pTutorial->mChangeText("AdlibDemoPerform" + std::to_string(m_textReadCnt));
		}
		else if (m_battleState == GameManager::eBattleState::eBattle){
			result = m_pTutorial->mChangeText("AdlibDemoBattle" + std::to_string(m_textReadCnt));
		}
		break;
	case eTutorialState::eAdlibPlay:
		result = m_pTutorial->mChangeText("AdlibPlay" + std::to_string(m_textReadCnt));
		break;
	case eTutorialState::ePlayerOnly:
		result = m_pTutorial->mChangeText("PlayerOnly" + std::to_string(m_textReadCnt));
		break;
	case eTutorialState::eFin:
		result = m_pTutorial->mChangeText("Fin" + std::to_string(m_textReadCnt));
		break;
	}

	//Falseなら値のリセット
	if (!result){
		m_pTutorial->mSetVisible(false);
		m_textReadCnt = 1;
		result = true;
		m_isTutorialPlay = false;
		if (m_tutorialState != eTutorialState::eFin){
			m_timeEngage = true;
		}
		m_prevWholeBeatNo = int(m_rhythm->mWholeBeatTime());
	}
	else{
		result = false;
	}
	
	return result;
}
void SceneTutorial::mRhythmicMotion(){
	m_pField->mRhythmicMotion();
	if (m_pOrderList){
		m_pOrderList->mRhythmicMotion();
	}
}

void SceneTutorial::mTimeEngagerForTuto(){
	//OFFなら実行しない
	if (!m_timeEngage)return;
	if (int(m_rhythm->mWholeBeatTime()) != m_prevWholeBeatNo){
		m_timeEngage = false;
	}

}


bool SceneTutorial::Updater(){
	if (m_isEndTransition && m_sound){
		m_sound->PlayToLoop();
		if (m_battleState != GameManager::eBattleState::eWin && m_battleState != GameManager::eBattleState::eLose){
			if (m_bgmVolume > 30){
				m_sound->SetValume(-m_bgmVolume * 100);
				m_bgmVolume--;
			}
		}
	}
	if (m_particle){
		m_particle->mUpdate(0.8);
	}
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager->mUpdate(1);
	}

	if (m_rhythm){
		m_rhythm->mAcquire();
	}
	if (m_pGauge){
		m_pGauge->mUpdate(1);
	}

	bool result = false;

	result = mTutorialUpdater();
	if (!result){
		mRhythmicMotion();
		return true;
	}

	//Tutorialによる時間のズレを起こさないための処理
	mTimeEngagerForTuto();
	if (m_timeEngage){
		mRhythmicMotion();
		return true;
	}


	if (m_processState == eGameState::ePreCountIn){
		
		m_processState = eGameState::eCountIn;
		m_prevWholeBeatNo = (int)(m_rhythm->mWholeBeatTime() + 0.1f);
	}
	else if (m_processState == eGameState::eCountIn){
		auto command = m_pActionBoard->mSelectType();
		if (command){
		}
		else{
			command = m_pActionBoard->mGetCommand(eMusical::eNull);
		}
		m_pField->mUpdate(command);
		mCountIn();
	}
	else if (m_processState == eGameState::eUpdate){
		switch (m_battleState)
		{
		case GameManager::eBattleState::eListen:
			mOnListen();
			break;
		case GameManager::eBattleState::ePerform:
			mOnPerform();
			break;
		case GameManager::eBattleState::eBattle:
			mOnBattle();
			break;
		case GameManager::eBattleState::eCheck:
			mCheckBattle();
			break;
		case GameManager::eBattleState::eResult:
			mOnResult();
			break;
		case GameManager::eBattleState::eNewWave:
			if (m_rhythm->mIsQuarterBeat()){
//				m_processState = eGameState::ePreCountIn;
				m_battleState = GameManager::eBattleState::eListen;
				m_enemyHp = &m_pBattleEnemyManager->mGetCharaStatus(m_waveID - 1);
				m_pGauge->mSetHpAll(&m_charaHp, m_enemyHp);
				m_pBattleEnemyManager->ResetEnemyList(m_waveID - 1, &m_view);
				m_isTutorialPlay = true;
			}
			break;
		default:
			break;
		}

	}
	else if (m_processState == eGameState::eFin){
		m_battleState = GameManager::eBattleState::eNull;
	}

	mRhythmicMotion();

	if (m_pOrderList){
		m_pOrderList->mUpdate();
	}
	m_players.mUpdate(0, eMusical::eNull);
	return true;
}

void SceneTutorial::Render(){
	m_view.Render();
	auto& shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	if (m_pField){
		m_pField->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	}
	m_players.mRender(shaderHash["texture"].get());
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager->mRender(shaderHash["texture"]);
	}
	if (m_particle){
		m_particle->mRender(shaderHash["texture"].get());
	}

	return;
}

void SceneTutorial::UIRender(){
	auto& shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	if (m_pOrderList){
		m_pOrderList->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	}
	if (m_pMessage){
		m_pMessage->mRender(shaderHash["transparent"].get());
	}
	if (m_pGauge){
		m_pGauge->mRender();
	}

	if (m_battleState == GameManager::eBattleState::eResult){
		if (m_pResult){
			m_pResult->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
		}
	}
		if (m_pTutorial){
			m_pTutorial->mRender(shaderHash["transparent"].get(),shaderHash["color"].get());
		}
		GameManager::mGetInstance().mfadeManager().mRender(shaderHash["color"].get());
	return;
}

bool SceneTutorial::TransitionIn(){
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		return kTransitionning;
	}

	return kTransitionEnd;
}

//
bool SceneTutorial::TransitionOut(){
	if (!GameManager::mGetInstance().mfadeManager().Out(1)){
		return kTransitionning;
	}
	m_isEndTransition = true;
	return kTransitionEnd;
}

void SceneTutorial::mOnResult(){

	if (!m_initUpdateProcess){
		m_pGauge.reset();
		m_pMessage.reset();

		m_sound = std::make_shared<GameSound>();
		m_sound->Load("Sound\\Result\\result.wav");
		m_rhythm->mInitializeRhythm(m_sound, 110);

		m_isTutorialPlay = true;
		m_tutorialState = eTutorialState::eFin;
		m_pTutorial->mShowBackCover(true);

		m_pResult = std::make_unique<ResultBoard>();
		m_pResult->mInitialize();

		m_pResult->mSetResultData(m_pOrderList->mGetResult(), m_winner, m_stageID);
		m_initUpdateProcess = true;

		m_pOrderList.reset();
		m_resultUpdateTime = 1.0f;
		m_bgmVolume = 60;

	}

	m_pResult->mUpdate(m_resultUpdateTime);
	if (m_pResult->mIsEnd()){
		const bool isPress = GameController::GetJoypad().ButtonPress(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger(VK_SPACE);
		if (isPress){
			ChangeScene(SceneGame::Name, LoadState::eUse);
		}
	}
	else{
		const bool isPress = GameController::GetJoypad().IsButtonDown(eJoyButton::eB) || GameController::GetKey().IsKeyDown(VK_SPACE);
		if (isPress){
			m_resultUpdateTime += 0.1f;
		}
	}
}

// 敵の演奏
void SceneTutorial::mOnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;
	// TODO: 敵の演奏をする処理＆スタックされる処理＆敵によってはオーダーリストに細工をする
	
	if (!m_initUpdateProcess){
		if (m_tutorialState != eTutorialState::ePlayerOnly){
			if (m_tutorialState == eTutorialState::eAdlibDemo || m_tutorialState == eTutorialState::eAdlibPlay){
				m_enemyVector.back() = m_pActionBoard->mGetCommand(eMusical::eAdlib);
			}
			m_pOrderList->mAddEnemyOrder(m_enemyVector);
		}
		else{
			m_enemyVector.clear();
			for (auto itr : m_pBattleEnemyManager->GetList()){
				m_enemyVector.push_back(m_pActionBoard->mGetCommand(itr));
			}
			m_pOrderList->mAddEnemyOrder(m_enemyVector);
			m_pOrderList->mSetOption(eAppendOption::eNone);
		}
		m_pOrderList->mPlay();
		m_initUpdateProcess = true;
	}

	//光る奴
	m_pField->mUpdate(m_pOrderList->mGetActionCommand());

	if (m_pOrderList->mIsEnd()){
		m_initUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::ePerform;
		if (m_tutorialState == eTutorialState::eFirstDemo || m_tutorialState == eTutorialState::eAdlibDemo){
			m_pOrderList->mSetTutorial(true);
			m_isTutorialPlay = true;
		}
	}

	return;
}

// プレイヤーの演奏
void SceneTutorial::mOnPerform(){
	if (m_battleState != GameManager::eBattleState::ePerform)return;
	// TODO: プレイヤーの演奏する処理

	if (!m_initUpdateProcess){
		m_pOrderList->mPlay();
		m_initUpdateProcess = true;
	}

	m_pField->mUpdate(m_pOrderList->mGetActionCommand());

	if (m_pOrderList->mIsEnd()){
		m_initUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::eBattle;

		m_pOrderList->mSetTutorial(false);
		if (m_tutorialState == eTutorialState::eFirstDemo || m_tutorialState == eTutorialState::eAdlibDemo){
			m_isTutorialPlay = true;
		}
	}

	return;
}

// 戦闘開始
void SceneTutorial::mOnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListenとOnPerformの結果を反映する処理
	if (!m_initUpdateProcess){
		m_pOrderList->mPlay();
		m_initUpdateProcess = true;
	}

	m_pField->mUpdate(m_pOrderList->mGetActionCommand());
	auto i = m_pOrderList->mGetDamage();
	if (i > 0){
		if (m_tutorialState == eTutorialState::eAdlibDemo || m_tutorialState == eTutorialState::eFirstDemo){	//Demoの場合はHPを削らない
		}else{
			m_enemyHp->_hp -= i;
		}
	}
	else if (i < 0){
		if (m_charaHp._hp > 1){
			m_charaHp._hp += i;
		}
	}

	if (m_pOrderList->mIsEnd()){
		m_initUpdateProcess = false;
		m_battleState = GameManager::eBattleState::eCheck;
		
		if (m_tutorialState == eTutorialState::eAdlibPlay){
			m_tutorialState = eTutorialState::ePlayerOnly;
			m_enemyHp->_hp = 0;
			m_charaHp._hp = m_charaHp._maxHp;
			m_pOrderList->mSetTutorial(false);
			m_pOrderList->mGetResult()._missCount = 0;
			m_pOrderList->mGetResult()._maxCount = 20;
			m_isTutorialPlay = true;
		}
	}
	return;
}

// 判定用
void SceneTutorial::mCheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: 勝負判定処理＆場合によってはWaveを進める処理
	m_pOrderList->mEndReset();

	if (m_enemyHp->_hp <= 0){
		if (m_waveID < m_MaxWave){
			m_waveID++;

			m_pBattleEnemyManager->misDie();
			m_particle = std::make_shared<AttackParticle>(m_particleDesc, &m_view);
			m_battleState = GameManager::eBattleState::eNewWave;
			m_processState = eGameState::ePreCountIn;
		}
		else{
			m_battleState = GameManager::eBattleState::eWin;
			m_processState = eGameState::ePreCountIn;
		}
	}
	else if (m_charaHp._hp <= 0){
		m_battleState = GameManager::eBattleState::eLose;
		m_processState = eGameState::ePreCountIn;
	}
	else{
		switch (m_tutorialState)
		{
		case eTutorialState::eFirstDemo:
			m_tutorialState = eTutorialState::eFirstPlay;
			m_isTutorialPlay = true;
			break;
		case eTutorialState::eFirstPlay:
			m_tutorialState = eTutorialState::eAdlibDemo;
			m_isTutorialPlay = true;
			break;
		case eTutorialState::eAdlibDemo:
			m_tutorialState = eTutorialState::eAdlibPlay;
			m_isTutorialPlay = true;
			break;
		case eTutorialState::eFin:
			break;
		default:
			break;
		}
		m_battleState = GameManager::eBattleState::eListen;
		m_processState = eGameState::ePreCountIn;
	}

}

void SceneTutorial::mCountIn(){
//	double Padding;
//	float exFrame = modf(m_rhythm->mWholeBeatTime(), &Padding);

	if (m_battleState == GameManager::eBattleState::eWin || m_battleState == GameManager::eBattleState::eLose){
		if (int(m_rhythm->mWholeBeatTime()) != m_prevWholeBeatNo){
			m_inCount = 0;
			m_pMessage->mSetActive(false);

			m_preInitProcess = false;
			m_winner = m_battleState;
			m_battleState = GameManager::eBattleState::eResult;
			m_sound->Stop();
		}
	}
	else if (m_battleState == GameManager::eBattleState::ePerform){
		if (int(m_rhythm->mWholeBeatTime() + 0.03f) != m_prevWholeBeatNo){
			m_inCount = 0;
			m_processState = eGameState::eUpdate;
			m_pMessage->mSetActive(false);
			m_preInitProcess = false;

			return;
		}
	}
	else if (m_battleState == GameManager::eBattleState::eResult){
		m_processState = eGameState::eUpdate;
		m_inCount = 0;
	}
	else {		//普段はこっち
		if (int(m_rhythm->mWholeBeatTime() + 0.1f) != m_prevWholeBeatNo){
			m_inCount = 0;
			m_pMessage->mSetActive(false);
			m_processState = eGameState::eUpdate;
			m_preInitProcess = false;
			return;
		}
	}
	if (m_rhythm->mIsQuarterBeat()){
		m_inCount++;
		//		printf("%d\n", m_inCount);
	}

	if (m_inCount >= 2){
		if (m_battleState == GameManager::eBattleState::eWin || m_battleState == GameManager::eBattleState::eLose){
			m_sound->SetValume(-m_bgmVolume * 100);
			m_bgmVolume++;
			m_pMessage->mChangeTexture(m_battleState);
		}
		else if (m_battleState == GameManager::eBattleState::eNewWave){
			if (m_waveID == 1){
				//tutorial
				m_pMessage->mWaveMessageOpen(0);
			}else{
				m_pMessage->mWaveMessageOpen(m_waveID - 1);
			}
		}
		else{
			if (!m_preInitProcess){
				m_pOrderList->mLinePlay();
				m_preInitProcess = true;
			}
			m_pMessage->mChangeTexture(m_battleState);
		}
		m_pMessage->mSetActive(true);

	}

}