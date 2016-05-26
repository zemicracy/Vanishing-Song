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
eMusical askey[8] = {	eMusical::eBlue, eMusical::eGreen, eMusical::eNull, eMusical::eGreen,
						eMusical::eBlue, eMusical::eYellow, eMusical::eNull, eMusical::eBlue	};

bool SceneBattle::Initialize(){
	Singleton<RhythmManager>::GetInstance().mInitializeRhythm(0, 110);
	
	RegisterScene(new SceneGame());

	m_battleState = GameManager::eBattleState::eListen;

	m_pMessage = std::make_unique<BattleMessage>();

	m_pActionBoard = std::make_shared<ActionBoard>();
	m_pActionBoard->mInitialize();

	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(GameManager::eGameMode::eEighter,m_battleState,m_pActionBoard);


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
	//最後に行う
	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(m_bgmVolume);
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
		default:
			break;
		}

	}
	else if (m_processState == eGameState::eFin){
		m_battleState = GameManager::eBattleState::eNull;
	}

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
	return;
}

void SceneBattle::UIRender(){
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pOrderList->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	m_pMessage->mRender(shaderHash["transparent"].get());
	m_pGauge->mRender();
	return;
}

bool SceneBattle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneBattle::TransitionOut(){
	return kTransitionEnd;
}



// 敵の演奏
void SceneBattle::mOnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;
	// TODO: 敵の演奏をする処理＆スタックされる処理＆敵によってはオーダーリストに細工をする

	if (!m_InitUpdateProcess){
		
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

	if (charaHp._hp <= 0){
		m_battleState = GameManager::eBattleState::eLose;
		m_processState = eGameState::ePreCountIn;
	}
	else if (enemyHp._hp <= 0){
		m_battleState = GameManager::eBattleState::eWin;
		m_processState = eGameState::ePreCountIn;
	}
	else{
		m_battleState = GameManager::eBattleState::eListen;
		m_processState = eGameState::ePreCountIn;
	}

}

void SceneBattle::mCountIn(){
	static int cnt = 0;
	double Padding;
	float exFrame = modf(m_rhythm->mWholeBeatTime(), &Padding);

	//特殊仕様　Prefromの時だけ
	if (m_battleState == GameManager::eBattleState::eWin || m_battleState == GameManager::eBattleState::eLose){
		if (int(m_rhythm->mWholeBeatTime()) != m_prevWholeBeatNo){
			const bool isPress = GameController::GetJoypad().ButtonRelease(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger(VK_SPACE);
			if (isPress){
				cnt = 0;
				m_pMessage->mSetActive(false);
				ChangeScene(SceneGame::Name, LoadState::eUse);
				Singleton<ResourceManager>::GetInstance().mGetBGM(0)->Stop();
			}
		}
	}
	else if (m_battleState == GameManager::eBattleState::ePerform){
		if (int(m_rhythm->mWholeBeatTime() + 0.03f) != m_prevWholeBeatNo){
			cnt = 0;
			m_processState = eGameState::eUpdate;
			m_pMessage->mSetActive(false);
			m_PreInitProcess = false;

			return;
		}
	}
	else {		//普段はこっち
		if (int(m_rhythm->mWholeBeatTime() + 0.1f) != m_prevWholeBeatNo){
			std::cout << m_prevWholeBeatNo <<" "<< int(m_rhythm->mWholeBeatTime() + 0.1f) <<std::endl;
			cnt = 0;
			m_pMessage->mSetActive(false);
			m_processState = eGameState::eUpdate;
			m_PreInitProcess = false;
			return;
		}
	}
	if (m_rhythm->mIsQuarterBeat()){
		cnt++;
//		printf("%d\n", cnt);
	}

	if (cnt >= 2){
		if (m_battleState == GameManager::eBattleState::eWin || m_battleState == GameManager::eBattleState::eLose){
			Singleton<ResourceManager>::GetInstance().mGetBGM(0)->SetValume(-m_bgmVolume*100);
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