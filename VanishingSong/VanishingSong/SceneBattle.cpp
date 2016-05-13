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

	return true;
}

void SceneBattle::Finalize(){
	m_pModelBase->Finalize();
	return;
}

bool SceneBattle::Updater(){
	
	return true;
}

void SceneBattle::Render(){
	m_view.Render();
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pModelBase->Render(shaderHash["texture"].get());
	return;
}

void SceneBattle::UIRender(){
	return;
}

bool SceneBattle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneBattle::TransitionOut(){
	return kTransitionEnd;
}