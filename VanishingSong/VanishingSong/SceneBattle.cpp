#include "SceneBattle.h"
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
eMusical askey[8] = {	eMusical::eBlue, eMusical::eYellow, eMusical::eNull, eMusical::eBlue,
						eMusical::eNull, eMusical::eRed, eMusical::eAdlib, eMusical::eAdlib };

bool SceneBattle::Initialize(){
	_heapmin();

	mLoadTextData();

	
	RegisterScene(new SceneGame());

	m_pMessage = std::make_unique<BattleMessage>();

	m_pActionBoard = std::make_shared<ActionBoard>();
	m_pActionBoard->mInitialize();


	//仮
	/*for (int i = 0; i < 8; i++){
		EnemyVector.push_back(m_pActionBoard->mGetCommand(askey[i]));
	}*/

	m_pField = std::make_unique<BattleField>();
	m_pField->mInitialize(&m_view,m_rhythm.get());

	m_pBattleEnemyManager = std::make_shared<BattleEnemyManager>();
	m_pBattleEnemyManager->mInitialize(&m_view, m_pField.get());

	//ウェーブ系
	m_MaxWave = m_pBattleEnemyManager->mGetWaveAllCount();
	m_nowWave = 1;
	m_pBattleEnemyManager->ResetEnemyList(m_nowWave-1, &m_view);

	m_pGauge = std::make_unique<GaugeManager>();
	m_pGauge->mInitialize();

	//hp
	m_enemyHp = &m_pBattleEnemyManager->mGetCharaStatus(m_nowWave - 1);
	m_pGauge->mSetHpAll(&m_charaHp, m_enemyHp);


	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(m_beatMode,m_battleState,m_pActionBoard.get(),m_pField.get(),m_rhythm.get());

	// プレイヤーの初期化
	/*for (auto& index : GameManager::mGetInstance().mGetUsePlayer()){
		auto gearframe = ResourceManager::mGetInstance().mGetPlayerHash(index.second);
		m_charaEntity.SetCamera(gearframe->m_pBody, &m_view);
		m_players.mSetPlayer(index.second, m_pField->mGetPlayerLane(index.second),gearframe);
		m_charaHp._maxHp = m_charaHp._hp += 8;
	}*/


	m_battleState = GameManager::eBattleState::eNewWave;
	m_processState = eGameState::ePreCountIn;
	m_InitUpdateProcess = false;
	m_PreInitProcess = false;
	m_prevWholeBeatNo = 0;

	m_bgmVolume = 30;
	m_inCount = 0;
	
		
	//最後に行う
	m_sound->SetValume(-m_bgmVolume*100);
	m_sound->PlayToLoop();
	_heapmin();
	return true;
}

void SceneBattle::Finalize(){
	_heapmin();
	if (m_pOrderList){
		m_pOrderList.reset();
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

	_heapmin();
	return;
}
void SceneBattle::mLoadTextData(){
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
		m_sound->Load("Sound\\Battle\\normal.wav");
	}

	m_rhythm = std::make_shared<RhythmManager>();
	m_rhythm->mInitializeRhythm(m_sound, 120);
	m_rhythm->mAcquire();


	cip.mUnLoad();
}


bool SceneBattle::Updater(){
	if (kCharaDebug){
		if (GameController::GetKey().IsKeyDown('I')){
			m_enemyHp->_hp -= 1;
		}
	}
	if (m_rhythm){
		m_rhythm->mAcquire();
	}
	if (m_battleState != GameManager::eBattleState::eWin && m_battleState != GameManager::eBattleState::eLose && m_battleState != GameManager::eBattleState::eResult){
		if (m_bgmVolume > 8){
			m_sound->SetValume(-m_bgmVolume * 100);
			m_bgmVolume--;
		}
	}

	if (m_processState == eGameState::ePreCountIn){
		//オプション設定
		if (m_battleState == GameManager::eBattleState::eListen){
			m_pOrderList->mSetOption(eAppendOption::eNone);
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
		case GameManager::eBattleState::eNewWave:
			if (m_rhythm->mIsQuarterBeat()){
				m_processState = eGameState::ePreCountIn;
				m_battleState = GameManager::eBattleState::eListen;
				m_enemyHp = &m_pBattleEnemyManager->mGetCharaStatus(m_nowWave - 1);
				m_pGauge->mSetHpAll(&m_charaHp, m_enemyHp);
				m_pBattleEnemyManager->ResetEnemyList(m_nowWave - 1, &m_view);

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
	if (m_pOrderList){
		m_pOrderList->mRender3D(shaderHash["texture"].get());
	}
	if (m_pBattleEnemyManager){
		m_pBattleEnemyManager->mRender(shaderHash["texture"]);
	}
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
		m_pGauge.reset();
		m_pMessage.reset();
		m_sound.reset();
		m_rhythm->mFinalize();
		m_rhythm.reset();
		

		m_pResult = std::make_unique<ResultBoard>();
		m_pResult->mInitialize();

		m_pResult->mSetResultData(m_pOrderList->mGetResult(),m_winner,m_stageID);
		m_InitUpdateProcess = true;

		m_pOrderList.reset();
	}

	m_pResult->mUpdate();
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
		/*for (int i = 0; i < 8; i++)
		{
			EnemyVector.push_back(m_pActionBoard->mGetCommand(askey[i]));
		}*/
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
		m_enemyHp->_hp -= i;
	}
	else if (i < 0){
		m_charaHp._hp += i;
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

	if (m_enemyHp->_hp <= 0){
		if (m_nowWave < m_MaxWave){
			m_nowWave++;

			m_pBattleEnemyManager->misDie();
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
				
				m_PreInitProcess = false;
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
			m_sound->SetValume(-m_bgmVolume*100);
			m_bgmVolume++;
			m_pMessage->mChangeTexture(m_battleState);
		}
		else if(m_battleState == GameManager::eBattleState::eNewWave){
			m_pMessage->mWaveMessageOpen(m_nowWave-1);
		}
		else{
			if (!m_PreInitProcess){
				m_pOrderList->mLinePlay();
				m_PreInitProcess = true;
			}
			m_pMessage->mChangeTexture(m_battleState);
		}
		m_pMessage->mSetActive(true);
		
	}

}