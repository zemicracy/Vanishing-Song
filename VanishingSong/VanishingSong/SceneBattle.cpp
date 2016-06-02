#include "SceneBattle.h"
#include <iostream>
#include<Skybox.h>
#include<Rectangle3D.h>
#include<PixelShader.h>
#include <GameController.h>
#include "ResourceManager.h"
#include"SceneGame.h"
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

std::vector<std::shared_ptr<ActionCommand>>EnemyVector;
eMusical askey[8] = {	eMusical::eBlue, eMusical::eNull, eMusical::eGreen, eMusical::eBlue,
						eMusical::eNull, eMusical::eRed, eMusical::eGreen, eMusical::eBlue	};

bool SceneBattle::Initialize(){
	Singleton<RhythmManager>::GetInstance().mInitializeRhythm(0, 110);
	
	m_inCount = 0;
	RegisterScene(new SceneGame());

	m_battleState = GameManager::eBattleState::eListen;

	m_pMessage = std::make_unique<BattleMessage>();

	m_pActionBoard = std::make_shared<ActionBoard>();
	m_pActionBoard->mInitialize();

	m_rhythm = &Singleton<RhythmManager>::GetInstance();
	m_rhythm->mAcquire();

	//仮
	for (int i = 0; i < 8; i++){
			EnemyVector.push_back(m_pActionBoard->mGetCommand(askey[i]));
	}

	m_pField = std::make_unique<BattleField>();
	m_pField->mInitialize(&m_view);

	m_pGauge = std::make_unique<GaugeManager>();
	m_pGauge->mInitialize();
	m_pGauge->mSetHpAll(&charaHp, &enemyHp);

	m_pResult = std::make_unique<ResultBoard>();
	m_pResult->mInitialize();

	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(GameManager::eGameMode::eEighter,m_battleState,m_pActionBoard.get(),m_pField.get());
	// プレイヤーの初期化
	for (auto& index : Singleton<GameManager>::GetInstance().mGetUsePlayer()){
		auto gearframe = Singleton<ResourceManager>::GetInstance().mGetPlayerHash(index.second);
		m_charaEntity.SetCamera(gearframe->m_pBody, &m_view);
		m_players.mSetPlayer(index.second, m_pField->mGetPlayerLane(index.second),gearframe);
	}

	m_processState = eGameState::ePreCountIn;
	m_InitUpdateProcess = false;
	m_PreInitProcess = false;
	m_prevWholeBeatNo = 0;

	m_bgmVolume = 0;
	charaHp._maxHp = charaHp._hp = 10;
	enemyHp._maxHp = enemyHp._hp = 20;

	m_pBattleEnemyManager = std::make_shared<BattleEnemyManager>();
	m_pBattleEnemyManager->Initialize(&m_view, m_pField.get());

	//最後に行う
	Singleton<ResourceManager>::GetInstance().mGetBGM(eMusical::eBlue)->SetValume(0);
	Singleton<ResourceManager>::GetInstance().mPlayBaseBGM(eMusical::eBlue);
	return true;
}

void SceneBattle::Finalize(){
	m_pOrderList.release();
	return;
}

bool SceneBattle::Updater(){
	m_rhythm->mAcquire();


	if (m_processState == eGameState::ePreCountIn){
		m_processState = eGameState::eCountIn;
		m_prevWholeBeatNo = (int)(m_rhythm->mWholeBeatTime()+0.1f);
	}
	else if (m_processState == eGameState::eCountIn){
		mCountIn();
	}
	else if (m_processState == eGameState::eUpdate){
		switch (m_battleState)
		{
		case GameManager::eBattleState::eWin:
			break;
		case GameManager::eBattleState::eLose:
			break;
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
		case GameManager::eBattleState::eNull:
			break;

		case GameManager::eBattleState::eResult:
			mOnResult();
			break;
		default:
			break;
		}

	}
	else if (m_processState == eGameState::eFin){
		m_battleState = GameManager::eBattleState::eNull;
	}

	m_pField->mRhythmicMotion();
	m_pGauge->mUpdate(1);
	m_pOrderList->mUpdate();
	m_players.mUpdate(0, eMusical::eNull);
	return true;
}

void SceneBattle::Render(){
	m_view.Render();
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pField->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	m_players.mRender(shaderHash["texture"].get());
	m_pOrderList->mRender3D(shaderHash["texture"].get());
	m_pBattleEnemyManager->mRender(shaderHash["texture"]);
	return;
}

void SceneBattle::UIRender(){
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pOrderList->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	m_pMessage->mRender(shaderHash["transparent"].get());
	m_pGauge->mRender();

	if (m_battleState == GameManager::eBattleState::eResult){
		m_pResult->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	}
	return;
}

bool SceneBattle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneBattle::TransitionOut(){
	return kTransitionEnd;
}

void SceneBattle::mOnResult(){

	if (!m_InitUpdateProcess){
		m_pResult->mSetResultData(m_pOrderList->mGetResult(),m_winner);
		m_InitUpdateProcess = true;
	}

	const bool isPress = GameController::GetJoypad().ButtonRelease(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger(VK_SPACE);
	if (isPress){
		ChangeScene(SceneGame::Name, LoadState::eUse);
	}
}

// 敵の演奏
void SceneBattle::mOnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;
	// TODO: 敵の演奏をする処理＆スタックされる処理＆敵によってはオーダーリストに細工をする

	if (!m_InitUpdateProcess){
		EnemyVector.clear();
		for (auto itr : m_pBattleEnemyManager->GetList()){
			EnemyVector.push_back(m_pActionBoard->mGetCommand(itr));
		}
		m_pOrderList->mAddEnemyOrder(EnemyVector);
		m_pOrderList->mPlay();
		m_InitUpdateProcess = true;
	}

	//光る奴
	m_pField->mUpdate(m_pOrderList->mGetActionCommand());

	if (m_pOrderList->mIsEnd()){
		m_InitUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::ePerform;
	}

	return;
}

// プレイヤーの演奏
void SceneBattle::mOnPerform(){
	if (m_battleState != GameManager::eBattleState::ePerform)return;
	// TODO: プレイヤーの演奏する処理

	if (!m_InitUpdateProcess){
		m_pOrderList->mPlay();
		m_InitUpdateProcess = true;
	}

	m_pField->mUpdate(m_pOrderList->mGetActionCommand());

	if (m_pOrderList->mIsEnd()){
		m_InitUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::eBattle;
	}

	return;
}

// 戦闘開始
void SceneBattle::mOnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListenとOnPerformの結果を反映する処理
	if (!m_InitUpdateProcess){
		m_pOrderList->mPlay();
		m_InitUpdateProcess = true;
	}

	m_pField->mUpdate(m_pOrderList->mGetActionCommand());
	auto i = m_pOrderList->mGetDamage();
	if (i > 0){
		enemyHp._hp -= i;
	}
	else if (i < 0){
		charaHp._hp += i;
	}

	if (m_pOrderList->mIsEnd()){
		m_InitUpdateProcess = false;
		m_battleState = GameManager::eBattleState::eCheck;
	}
	return;
}

// 判定用
void SceneBattle::mCheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: 勝負判定処理＆場合によってはWaveを進める処理
	m_pOrderList->mEndReset();

	if (enemyHp._hp <= 0){
		m_battleState = GameManager::eBattleState::eWin;
		m_processState = eGameState::ePreCountIn;
	}
	else if (charaHp._hp <= 0){
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

	//特殊仕様　Prefromの時だけ
	if (m_battleState == GameManager::eBattleState::eWin || m_battleState == GameManager::eBattleState::eLose){
		if (int(m_rhythm->mWholeBeatTime()) != m_prevWholeBeatNo){
			const bool isPress = GameController::GetJoypad().ButtonRelease(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger(VK_SPACE);
			if (isPress){
				m_inCount = 0;
				m_pMessage->mSetActive(false);
				
				m_PreInitProcess = false;
				m_winner = m_battleState;
				m_battleState = GameManager::eBattleState::eResult;
				Singleton<ResourceManager>::GetInstance().mGetBGM(eMusical::eBlue)->Stop();
			}
		}
	}
	else if (m_battleState == GameManager::eBattleState::ePerform){
		if (int(m_rhythm->mWholeBeatTime() + 0.03f) != m_prevWholeBeatNo){
			m_inCount = 0;
			m_processState = eGameState::eUpdate;
			m_pMessage->mSetActive(false);
			m_PreInitProcess = false;

			return;
		}
	}
	else if (m_battleState == GameManager::eBattleState::eResult){
		m_processState = eGameState::eUpdate;
		m_inCount = 0;
	}
	else {		//普段はこっち
		if (int(m_rhythm->mWholeBeatTime() + 0.1f) != m_prevWholeBeatNo){
			std::cout << m_prevWholeBeatNo <<" "<< int(m_rhythm->mWholeBeatTime() + 0.1f) <<std::endl;
			m_inCount = 0;
//			std::cout << m_prevWholeBeatNo <<" "<< int(m_rhythm->mWholeBeatTime() + 0.1f) <<std::endl;
			m_inCount = 0;
			m_pMessage->mSetActive(false);
			m_processState = eGameState::eUpdate;
			m_PreInitProcess = false;
			return;
		}
	}
	if (m_rhythm->mIsQuarterBeat()){
		m_inCount++;
//		printf("%d\n", m_inCount);
	}

	if (m_inCount >= 2){
		if (m_battleState == GameManager::eBattleState::eWin || m_battleState == GameManager::eBattleState::eLose){
			Singleton<ResourceManager>::GetInstance().mGetBGM(eMusical::eBlue)->SetValume(-m_bgmVolume*100);
			m_bgmVolume++;
		}else{
			if (!m_PreInitProcess){
				m_pOrderList->mLinePlay();
				m_PreInitProcess = true;
			}
		}
		m_pMessage->mChangeTexture(m_battleState);
		m_pMessage->mSetActive(true);
		
	}

}