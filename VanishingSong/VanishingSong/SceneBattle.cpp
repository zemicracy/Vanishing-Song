#include "SceneBattle.h"
#include <iostream>
#include<Skybox.h>
#include<Rectangle3D.h>
#include<PixelShader.h>
#include <GameController.h>
#include "ResourceManager.h"

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



bool SceneBattle::Initialize(){
	Singleton<RhythmManager>::GetInstance().mInitializeRhythm(0, 110);

	m_pTexture = std::make_unique<Texture>();
	m_pTexture->Load("Texture\\GameBack.jpg");

	m_pModelBase = std::make_unique<Skybox>();
	m_pModelBase->Initialize();
	m_pModelBase->SetCamera(&m_view);
	m_pModelBase->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture("skybox").get());

	m_battleState = GameManager::eBattleState::eListen;

	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(GameManager::eGameMode::eEighter,m_battleState);

	m_pActionBoard = std::make_unique<ActionBoard>();
	m_pActionBoard->mInitialize();

	m_rhythm = &Singleton<RhythmManager>::GetInstance();
	m_rhythm->mAcquire();


	m_processState = eGameState::ePreCountIn;
	m_InitUpdateProcess = false;
	m_prevWholeBeatNo = 0;

	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(0);
	return true;
}

void SceneBattle::Finalize(){
	m_pModelBase->Finalize();
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
		mOnListen();
		mOnPerform();
		mOnBattle();
		mCheckBattle();

	}
	else if (m_processState == eGameState::eFin){
		m_battleState = GameManager::eBattleState::eNull;
	}
	m_pOrderList->mUpdate();
	return true;
}

void SceneBattle::Render(){
	m_view.Render();
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pModelBase->Render(shaderHash["transparent"].get());
	
	return;
}

void SceneBattle::UIRender(){
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pOrderList->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	return;
}

bool SceneBattle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneBattle::TransitionOut(){
	return kTransitionEnd;
}

//interim
	std::vector<std::shared_ptr<ActionCommand>>EnemyVector;
	bool askey[8] = { 1, 1, 0, 1, 0, 0, 1, 1 };

// 敵の演奏
void SceneBattle::mOnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;
	// TODO: 敵の演奏をする処理＆スタックされる処理＆敵によってはオーダーリストに細工をする

	if (!m_InitUpdateProcess){
		EnemyVector.reserve(8);
		for (int i = 0; i < 8; i++){
			if (askey[i]){
				EnemyVector.push_back(m_pActionBoard->mGetCommand(eMusical::eBlue));
			}
			else{
				EnemyVector.push_back(m_pActionBoard->mGetCommand(eMusical::eNull));
			}
		}
		m_pOrderList->mAddEnemyOrder(EnemyVector);
		EnemyVector.clear();
		m_pOrderList->mPlay();
		m_InitUpdateProcess = true;
	}
	if (m_pOrderList->mIsEnd()){
		m_InitUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
	}

	return;
}

// プレイヤーの演奏
void SceneBattle::mOnPerform(){
	if (m_battleState != GameManager::eBattleState::ePerform)return;
	// TODO: プレイヤーの演奏する処理
	return;
}

// 戦闘開始
void SceneBattle::mOnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListenとOnPerformの結果を反映する処理

	return;
}

// 判定用
void SceneBattle::mCheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: 勝負判定処理＆場合によってはWaveを進める処理
}

void SceneBattle::mCountIn(){
	static int cnt = 0;
	double Padding;
	float exFrame = modf(m_rhythm->mWholeBeatTime(), &Padding);

	if (int(m_rhythm->mWholeBeatTime() + 0.1f) != m_prevWholeBeatNo){
//		std::cout << m_prevWholeBeatNo <<" "<< int(m_rhythm->mWholeBeatTime() + 0.1f) <<std::endl;
		cnt = 0;
		m_processState = eGameState::eUpdate;
		Debug::mPrint("Play!!");
	}
	if (m_rhythm->mIsQuarterBeat()){
		cnt++;
//		printf("%d\n", cnt);
	}

	if (cnt >= 2){
		Debug::mPrint("Listen");
	}

}