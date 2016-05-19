#include "SceneBattle.h"
#include <iostream>
#include<Rectangle3D.h>
#include<PixelShader.h>
#include <GameController.h>
#include "ResourceManager.h"

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
	m_pTexture = std::make_unique<Texture>();
	m_pTexture->Load("Texture/BattleImage.png");

	m_pModelBase = std::make_unique<Rectangle3D>();
	m_pModelBase->Initialize();
	m_pModelBase->SetCamera(&m_view);
	m_pModelBase->property._transform._translation = Vector3(0, 0, 0);
	m_pModelBase->property._color = Color(0, 0, 0, 1);
	m_pModelBase->SetTexture(m_pTexture.get());

	m_battleState = GameManager::eBattleState::eListen;

	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(GameManager::eGameMode::eQuarter);
	m_pOrderList->mSetFaze(m_battleState);


	return true;
}

void SceneBattle::Finalize(){
	m_pModelBase->Finalize();
	m_pOrderList.release();
	return;
}

bool SceneBattle::Updater(){
	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(0);

	OnListen();
	OnPerform();
	OnBattle();
	CheckBattle();
	return true;
}

void SceneBattle::Render(){
	m_view.Render();
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pModelBase->Render(shaderHash["texture"].get());
	

	return;
}

void SceneBattle::UIRender(){
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pOrderList->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	return;
}

bool SceneBattle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneBattle::TransitionOut(){
	return kTransitionEnd;
}

// 敵の演奏
void SceneBattle::OnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;

	// TODO: 敵の演奏をする処理＆スタックされる処理＆敵によってはオーダーリストに細工をする
	
	return;
}

// プレイヤーの演奏
void SceneBattle::OnPerform(){
	if (m_battleState != GameManager::eBattleState::ePerform)return;
	// TODO: プレイヤーの演奏する処理
	return;
}

// 戦闘開始
void SceneBattle::OnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListenとOnPerformの結果を反映する処理

	return;
}

// 判定用
void SceneBattle::CheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: 勝負判定処理＆場合によってはWaveを進める処理
}