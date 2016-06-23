#include "SceneBattle.h"
#include <iostream>
#include<Skybox.h>
#include<Rectangle3D.h>
#include<PixelShader.h>
#include <GameController.h>
#include "ResourceManager.h"
#include"SceneGame.h"
#include"Cipher.h"
#include"ModelUtility.h"
//debug
using namespace aetherClass;
const std::string SceneBattle::Name = "Battle";
SceneBattle::SceneBattle():
GameScene(Name, GetManager())
{
}

SceneBattle::~SceneBattle()
{
}


//interim
bool SceneBattle::Initialize(){
	_heapmin();

	mLoadTextData();

	
	RegisterScene(new SceneGame());

	m_pMessage = std::make_unique<BattleMessage>();

	m_pActionBoard = std::make_shared<ActionBoard>();
	m_pActionBoard->mInitialize();

	bool bossStage = m_stageID == 5 ? true : false;
	m_pField = std::make_unique<BattleField>();
	m_pField->mInitialize(&m_view,m_rhythm.get(),bossStage,m_MaxWave);

	m_pBattleEnemyManager = std::make_shared<BattleEnemyManager>();
	m_pBattleEnemyManager->mInitialize(&m_view, m_pField.get());

	m_pNotifier = std::make_unique<OptionNotifier>();
	m_pNotifier->mInitialize(&m_view, m_pField->mGetEnemyLane(eMusical::eBlue));

	//ウェーブ系
	m_waveID = 1;
	m_pBattleEnemyManager->ResetEnemyList(m_waveID-1, &m_view);

	m_pGauge = std::make_unique<GaugeManager>();
	m_pGauge->mInitialize(m_rhythm.get());

	//hp
	m_enemyHp = &m_pBattleEnemyManager->mGetCharaStatus(m_waveID - 1);
	m_pGauge->mSetHpAll(&m_charaHp, m_enemyHp);


	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(m_beatMode,m_battleState,m_pActionBoard.get(),m_pField.get(),m_rhythm.get(),m_stageID);

	// プレイヤーの初期化
	for (auto& index : GameManager::mGetInstance().mGetUsePlayer()){
		auto gearframe = ResourceManager::mGetInstance().mGetPlayerHash(index.second);
		gearframe->SetCamera(&m_view);
		m_players.mSetPlayer(index.second, m_pField->mGetPlayerLane(index.second),gearframe);
		m_charaHp._maxHp = m_charaHp._hp += 8;
	}


	m_battleState = GameManager::eBattleState::eNewWave;
	m_processState = eGameState::ePreCountIn;
	m_initUpdateProcess = false;
	m_preInitProcess = false;
	m_isEndTransition = false;
	m_prevWholeBeatNo = 0;

	m_bgmVolume = 60;
	m_inCount = 0;

	m_pField->mSetStageID(m_stageID);


	m_particleDesc._spawnRate = 10;
	m_particleDesc._scale = 5;
	m_particleDesc._texturePath = "Texture\\OrderList\\note_a.png";
	m_particleDesc._rangeMin = Vector3(0, 0, 0);
	m_particleDesc._rangeMax = Vector3(20, 0, 20);
	m_particleDesc._startPoint = m_pField->mGetEnemyLane(eMusical::eAdlib);
	m_particleDesc._endPoint = m_pField->mGetEnemyLane(eMusical::eAdlib);
	m_particleDesc._endPoint._y += 100;

	m_players.mUpdate(0, eMusical::eNull);
	m_pOrderList->mUpdate();

	//最後に行う
	m_sound->SetValume(-m_bgmVolume*100);
	_heapmin();
	return true;
}

void SceneBattle::Finalize(){
	_heapmin();

	if (m_pOrderList){
		m_pOrderList.reset();
		m_pOrderList = nullptr;
	}
	if (m_sound){
		m_sound.reset();
		m_sound = nullptr;
	}
	if (m_pActionBoard){
		m_pActionBoard.reset();
		m_pActionBoard = nullptr;
	}
	if (m_pField){
		m_pField.reset();
		m_pField = nullptr;
	}
	if (m_pGauge){
		m_pGauge.reset();
		m_pGauge = nullptr;
	}
	if (m_pMessage){
		m_pMessage.reset();
		m_pMessage = nullptr;
	}
	if (m_pResult){
		m_pResult.reset();
		m_pResult = nullptr;
	}
	if (m_rhythm){
		m_rhythm->mFinalize();
		m_rhythm = nullptr;
	}
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager.reset();
		m_pBattleEnemyManager = nullptr;
	}
	if (m_particle){
		m_particle.reset();
		m_particle = nullptr;
	}
	if (m_pNotifier){
		m_pNotifier.reset();
		m_pNotifier = nullptr;
	}

	_heapmin();
	return;
}
void SceneBattle::mLoadTextData(){
	std::string file = GameManager::mGetInstance().mBattleDataFile();
	Cipher cip(file);
	
	m_stageID = std::atoi(&cip.mGetSpriteArray("[Stage]").front().front());
	m_MaxWave = std::atoi(&cip.mGetSpriteArray("[WaveAll]").front().front());
	int type = std::atoi(&cip.mGetSpriteArray("[Beat]").front().front());
	if (type == 4){
		m_beatMode = GameManager::eGameMode::eQuarter;
	}
	else{
		m_beatMode = GameManager::eGameMode::eEighter;
	}

	//サウンドのデータも読み込む
	m_sound = std::make_shared<GameSound>();
	m_rhythm = std::make_shared<RhythmManager>();
	if (m_stageID != 5){
		m_sound->Load("Sound\\Battle\\normal.wav");
		m_rhythm->mInitializeRhythm(m_sound.get(), 120);
	}
	else{
		m_sound->Load("Sound\\Battle\\boss.wav");
		m_rhythm->mInitializeRhythm(m_sound.get(), 117);
	}

	m_rhythm->mAcquire();


	cip.mUnLoad();
}


bool SceneBattle::Updater(){
	if (m_isEndTransition && m_sound){
			m_sound->PlayToLoop();
		if (m_battleState != GameManager::eBattleState::eWin && m_battleState != GameManager::eBattleState::eLose){
			if (m_bgmVolume > 30){
				m_sound->SetValume(-m_bgmVolume * 100);
				m_bgmVolume--;
			}
		}
	}
	if (kCharaDebug){
		if (GameController::GetKey().IsKeyDown('I')){
			m_enemyHp->_hp -= 1;
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
	if (m_pOrderList){
		m_pOrderList->mRhythmicMotion();
	}

	if (m_processState == eGameState::ePreCountIn){
		//オプション設定と敵のコマンド発行
		if (m_battleState == GameManager::eBattleState::eListen){
			m_enemyVector.clear();
			for (auto itr : m_pBattleEnemyManager->GetList()){
				m_enemyVector.push_back(m_pActionBoard->mGetCommand(itr));
			}
			m_pOrderList->mAddEnemyOrder(m_enemyVector);
			int option = m_pBattleEnemyManager->mGetAppendOption();
			m_pOrderList->mSetOption(option);
			if (option & 1){
				m_pNotifier->mChangeTexture(OrderList::eAppendOption::eBlack);
				m_pNotifier->mSetActive(true);
			}
			else if (option & 2){
				m_pNotifier->mChangeTexture(OrderList::eAppendOption::eReverce);
				m_pNotifier->mSetActive(true);
			}
		}
		m_processState = eGameState::eCountIn;
		m_prevWholeBeatNo = (int)(m_rhythm->mWholeBeatTime()+0.1f);
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
		m_pNotifier->mSetActive(false);
		//分岐
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
				m_processState = eGameState::ePreCountIn;
				m_battleState = GameManager::eBattleState::eListen;
				m_enemyHp = &m_pBattleEnemyManager->mGetCharaStatus(m_waveID - 1);
				m_pGauge->mSetHpAll(&m_charaHp, m_enemyHp);
				m_pBattleEnemyManager->ResetEnemyList(m_waveID - 1, &m_view);

			}
			break;
		default:
			break;
		}

	}
	else if (m_processState == eGameState::eFin){
		m_battleState = GameManager::eBattleState::eNull;
	}

	m_pField->mRhythmicMotion();
	if (m_pGauge){
		m_pGauge->mUpdate(1);
	}
	if (m_pOrderList){
		m_pOrderList->mUpdate();
		m_pOrderList->mRhythmicMotion();
	}
	m_players.mUpdate(0, eMusical::eNull);
	return true;
}

void SceneBattle::Render(){
	m_view.Render();
	auto& shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	if (m_pField){
		m_pField->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	}
		m_players.mRender(shaderHash["texture"].get());
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager->mRender(shaderHash["transparent"]);
	}
	if (m_particle){
		m_particle->mRender(shaderHash["texture"].get());
	}
	m_pNotifier->mRender(shaderHash["transparent"].get());

	return;
}

void SceneBattle::UIRender(){
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
	GameManager::mGetInstance().mfadeManager().mRender(shaderHash["color"].get());
	return;
}

bool SceneBattle::TransitionIn(){
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		return kTransitionning;
	}

	return kTransitionEnd;
}

//
bool SceneBattle::TransitionOut(){
	if (!GameManager::mGetInstance().mfadeManager().Out(1)){
		return kTransitionning;
	}
	m_isEndTransition = true;
	return kTransitionEnd;
}

void SceneBattle::mOnResult(){

	if (!m_initUpdateProcess){
		m_pGauge.reset();
		m_pMessage.reset();
		m_sound.reset();
		
		m_sound = std::make_shared<GameSound>();
		m_sound->Load("Sound\\Result\\result.wav");
		m_rhythm->mInitializeRhythm(m_sound.get(), 110);

		m_pResult = std::make_unique<ResultBoard>();
		m_pResult->mInitialize();

		m_pResult->mSetResultData(m_pOrderList->mGetResult(),m_winner,m_stageID);
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
void SceneBattle::mOnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;
	// TODO: 敵の演奏をする処理＆スタックされる処理＆敵によってはオーダーリストに細工をする

	if (!m_initUpdateProcess){
		m_pOrderList->mPlay();
		m_initUpdateProcess = true;
	}

	//光る奴
	m_pField->mUpdate(m_pOrderList->mGetActionCommand());


	m_pBattleEnemyManager->mChangeAnimation(eBattleActionType::eAttack, m_pOrderList->mGetActionCommand()->mGetType());
	if (m_pOrderList->mIsEnd()){
		m_initUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::ePerform;
	}

	return;
}

// プレイヤーの演奏
void SceneBattle::mOnPerform(){
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
	}

	return;
}

// 戦闘開始
void SceneBattle::mOnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListenとOnPerformの結果を反映する処理
	if (!m_initUpdateProcess){
		m_pOrderList->mPlay();
		m_initUpdateProcess = true;
	}

	m_pField->mUpdate(m_pOrderList->mGetActionCommand());
	auto i = m_pOrderList->mGetDamage();
	if (i > 0){
		m_enemyHp->_hp -= i;
		m_pBattleEnemyManager->mChangeAnimation(eBattleActionType::eDamage, eMusical::eMiss);
	}
	else if (i < 0){
		m_charaHp._hp += i;
//		m_pBattleEnemyManager->mChangeAnimation(eBattleActionType::eDamage, eMusical::eMiss);
	}

	if (m_pOrderList->mIsEnd()){
		m_initUpdateProcess = false;
		m_battleState = GameManager::eBattleState::eCheck;
	}
	return;
}

// 判定用
void SceneBattle::mCheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: 勝負判定処理＆場合によってはWaveを進める処理
	m_pOrderList->mEndReset();

	if (m_enemyHp->_hp <= 0){
		if (m_waveID < m_MaxWave){
			m_waveID++;

			m_pBattleEnemyManager->misDie();
			m_particle = std::make_shared<AttackParticle>(m_particleDesc,&m_view);


			m_pField->mDeleteWaveNote();
			m_battleState = GameManager::eBattleState::eNewWave;
			m_processState = eGameState::ePreCountIn;
		}
		else{
			m_pBattleEnemyManager->misDie();
			m_particle = std::make_shared<AttackParticle>(m_particleDesc, &m_view);
			m_pField->mDeleteWaveNote();

			m_battleState = GameManager::eBattleState::eWin;
			m_processState = eGameState::ePreCountIn;
		}
	}
	else if (m_charaHp._hp <= 0){
		m_battleState = GameManager::eBattleState::eLose;
		m_processState = eGameState::ePreCountIn;
	}
	else{
		m_battleState = GameManager::eBattleState::eListen;
		m_processState = eGameState::ePreCountIn;
	}

}

void SceneBattle::mCountIn(){
	double Padding;
	float exFrame = modf(m_rhythm->mWholeBeatTime(), &Padding);
//	std::cout << m_rhythm->mWholeBeatTime() << "\tstate\t" << (int)m_battleState << std::endl;

	if (m_battleState == GameManager::eBattleState::eWin || m_battleState == GameManager::eBattleState::eLose){
		if (int(m_rhythm->mWholeBeatTime()) != m_prevWholeBeatNo){
				m_inCount = 0;
				m_pMessage->mSetActive(false);
				
				m_preInitProcess = false;
				m_initUpdateProcess = false;
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
			m_sound->SetValume(-m_bgmVolume*100);
			m_bgmVolume++;
			m_pMessage->mChangeTexture(m_battleState);
		}
		else if(m_battleState == GameManager::eBattleState::eNewWave){
			m_pMessage->mWaveMessageOpen(m_waveID);
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