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

// �G�̉��t
void SceneBattle::OnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;

	// TODO: �G�̉��t�����鏈�����X�^�b�N����鏈�����G�ɂ���Ă̓I�[�_�[���X�g�ɍ׍H������
	
	return;
}

// �v���C���[�̉��t
void SceneBattle::OnPerform(){
	if (m_battleState != GameManager::eBattleState::ePerform)return;
	// TODO: �v���C���[�̉��t���鏈��
	return;
}

// �퓬�J�n
void SceneBattle::OnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListen��OnPerform�̌��ʂ𔽉f���鏈��

	return;
}

// ����p
void SceneBattle::CheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: �������菈�����ꍇ�ɂ���Ă�Wave��i�߂鏈��
}