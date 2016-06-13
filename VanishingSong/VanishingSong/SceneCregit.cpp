#include "SceneCregit.h"
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
const std::string SceneCregit::Name = "Cregit";
SceneCregit::SceneCregit() :
GameScene(Name, GetManager())
{
}

SceneCregit::~SceneCregit()
{
}


//interim
bool SceneCregit::Initialize(){
	_heapmin();

	mLoadTextData();


	RegisterScene(new SceneGame());

	m_pMessage = std::make_unique<BattleMessage>();

	m_pActionBoard = std::make_shared<ActionBoard>();
	m_pActionBoard->mInitialize();

	m_pField = std::make_unique<BattleField>();
	m_pField->mInitialize(&m_view, m_rhythm.get());

	m_pBattleEnemyManager = std::make_shared<BattleEnemyManager>();
	m_pBattleEnemyManager->mInitialize(&m_view, m_pField.get());

	//ウェーブ系
	m_MaxWave = m_pBattleEnemyManager->mGetWaveAllCount();
	m_waveID = 1;
	m_pBattleEnemyManager->ResetEnemyList(m_waveID - 1, &m_view);


	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(m_beatMode, m_battleState, m_pActionBoard.get(), m_pField.get(), m_rhythm.get());

	// プレイヤーの初期化
	for (auto& index : GameManager::mGetInstance().mGetUsePlayer()){
		auto gearframe = ResourceManager::mGetInstance().mGetPlayerHash(index.second);
		gearframe->SetCamera(&m_view);
		m_players.mSetPlayer(index.second, m_pField->mGetPlayerLane(index.second), gearframe);
	}


	m_battleState = GameManager::eBattleState::eNewWave;
	m_processState = eGameState::ePreCountIn;
	m_initUpdateProcess = false;
	m_preInitProcess = false;
	m_prevWholeBeatNo = 0;

	m_bgmVolume = 30;
	m_inCount = 0;


	m_particleDesc._spawnRate = 10;
	m_particleDesc._scale = 5;
	m_particleDesc._texturePath = "Texture\\OrderList\\note_a.png";
	m_particleDesc._rangeMin = Vector3(0, 0, 0);
	m_particleDesc._rangeMax = Vector3(20, 0, 20);
	m_particleDesc._startPoint = m_pField->mGetEnemyLane(eMusical::eBlue);
	m_particleDesc._endPoint = m_pField->mGetEnemyLane(eMusical::eBlue);
	m_particleDesc._endPoint._y += 100;

	//最後に行う
	m_sound->SetValume(-m_bgmVolume * 100);
	m_sound->PlayToLoop();
	_heapmin();
	return true;
}

void SceneCregit::Finalize(){
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
	if (m_pMessage){
		m_pMessage.reset();
		m_pMessage = nullptr;
	}
	if (m_rhythm){
		m_rhythm->mFinalize();
		m_rhythm = nullptr;
	}
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager.reset();
		m_pBattleEnemyManager = nullptr;
	}

	_heapmin();
	return;
}
void SceneCregit::mLoadTextData(){
	std::string file = GameManager::mGetInstance().mBattleDataFile();
	Cipher cip(file);

	m_stageID = std::atoi(&cip.mGetSpriteArray("[Stage]").front().front());
	int type = std::atoi(&cip.mGetSpriteArray("[Beat]").front().front());
	if (type == 4){
		m_beatMode = GameManager::eGameMode::eQuarter;
	}
	else{
		m_beatMode = GameManager::eGameMode::eEighter;
	}

	//サウンドのデータも読み込む
	m_sound = std::make_shared<GameSound>();
	if (m_stageID != 5){
		m_sound->Load("Sound\\Battle\\normal.wav");
	}
	else{
		m_sound->Load("Sound\\Battle\\boss.wav");
	}

	m_rhythm = std::make_shared<RhythmManager>();
	m_rhythm->mInitializeRhythm(m_sound, 120);
	m_rhythm->mAcquire();


	cip.mUnLoad();
}


bool SceneCregit::Updater(){
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager->mUpdate(1);
	}
	if (m_particle){
		m_particle->mUpdate(0.8);
	}
	if (m_rhythm){
		m_rhythm->mAcquire();
	}
	if (m_pOrderList){
		m_pOrderList->mRhythmicMotion();
	}
	if (m_battleState != GameManager::eBattleState::eWin && m_battleState != GameManager::eBattleState::eLose && m_battleState != GameManager::eBattleState::eResult){
		if (m_bgmVolume > 8){
			m_sound->SetValume(-m_bgmVolume * 100);
			m_bgmVolume--;
		}
	}

	if (m_processState == eGameState::ePreCountIn){
		//オプション設定と敵のコマンド発行
		if (m_battleState == GameManager::eBattleState::eListen){
			m_enemyVector.clear();
			for (auto itr : m_pBattleEnemyManager->GetList()){
				m_enemyVector.push_back(m_pActionBoard->mGetCommand(itr));
			}
			m_pOrderList->mAddEnemyOrder(m_enemyVector);
			m_pOrderList->mSetOption(eAppendOption::eNone);
		}
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
			break;
		case GameManager::eBattleState::eNewWave:
			if (m_rhythm->mIsQuarterBeat()){
				m_processState = eGameState::ePreCountIn;
				m_battleState = GameManager::eBattleState::eListen;
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
	if (m_pOrderList){
		m_pOrderList->mUpdate();
		m_pOrderList->mRhythmicMotion();
	}
	m_players.mUpdate(0, eMusical::eNull);
	return true;
}

void SceneCregit::Render(){
	m_view.Render();
	auto& shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	if (m_pField){
		m_pField->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	}
	m_players.mRender(shaderHash["texture"].get());
	if (m_pOrderList){
		m_pOrderList->mRender3D(shaderHash["texture"].get());
	}
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager->mRender(shaderHash["transparent"]);
	}

	if (m_particle){
		m_particle->mRender(shaderHash["texture"].get());
	}
	return;
}

void SceneCregit::UIRender(){
	auto& shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	if (m_pOrderList){
		m_pOrderList->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	}
	if (m_pMessage){
		m_pMessage->mRender(shaderHash["transparent"].get());
	}

	return;
}

bool SceneCregit::TransitionIn(){
	return kTransitionEnd;
}

bool SceneCregit::TransitionOut(){
	return kTransitionEnd;
}

// 敵の演奏
void SceneCregit::mOnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;
	// TODO: 敵の演奏をする処理＆スタックされる処理＆敵によってはオーダーリストに細工をする

	if (!m_initUpdateProcess){
		m_pOrderList->mPlay();
		m_initUpdateProcess = true;
	}

	//光る奴
	m_pField->mUpdate(m_pOrderList->mGetActionCommand());

	if (m_pOrderList->mIsEnd()){
		m_initUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::ePerform;
	}

	return;
}

// プレイヤーの演奏
void SceneCregit::mOnPerform(){
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
void SceneCregit::mOnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListenとOnPerformの結果を反映する処理
	if (!m_initUpdateProcess){
		m_pOrderList->mPlay();
		m_initUpdateProcess = true;
	}

	m_pField->mUpdate(m_pOrderList->mGetActionCommand());

	if (m_pOrderList->mIsEnd()){
		m_initUpdateProcess = false;
		m_battleState = GameManager::eBattleState::eCheck;
	}
	return;
}

// 判定用
void SceneCregit::mCheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: 勝負判定処理＆場合によってはWaveを進める処理
	m_pOrderList->mEndReset();

	if (m_waveID < m_MaxWave){
		m_waveID++;
		m_pBattleEnemyManager->misDie();
		m_particle = std::make_shared<AttackParticle>(m_particleDesc, &m_view);

		m_battleState = GameManager::eBattleState::eNewWave;
		m_processState = eGameState::ePreCountIn;
	}
	else{
		m_pBattleEnemyManager->misDie();
		m_particle = std::make_shared<AttackParticle>(m_particleDesc, &m_view);

		m_battleState = GameManager::eBattleState::eWin;
		m_processState = eGameState::ePreCountIn;
	}
}


void SceneCregit::mCountIn(){
	double Padding;
	float exFrame = modf(m_rhythm->mWholeBeatTime(), &Padding);
	//	std::cout << m_rhythm->mWholeBeatTime() << "\tstate\t" << (int)m_battleState << std::endl;

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