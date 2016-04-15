#include "SceneBattle.h"
#include <iostream>
#include<Rectangle3D.h>
#include<PixelShader.h>
#include <GameController.h>

#include "SceneGameover.h"
using namespace aetherClass;

SceneBattle::SceneBattle():
GameScene("Battle", GetManager())
{
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Initialize(){
	m_pTitleView = std::make_unique<ViewCamera>();
	m_pTitleView->property._translation = Vector3(0, 0, -3);

	m_pTexture = std::make_unique<Texture>();
	m_pTexture->Load("Texture/BattleImage.png");

	ShaderDesc desc;
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader\\VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader\\ColorTexture.hlsl";

	m_pShaderBase = std::make_unique<PixelShader>();
	m_pShaderBase->Initialize(desc, ShaderType::eVertex | ShaderType::ePixel);

	m_pModelBase = std::make_unique<Rectangle3D>();
	m_pModelBase->Initialize();
	m_pModelBase->SetCamera(m_pTitleView.get());
	m_pModelBase->property._transform._translation = Vector3(0, 0, 0);
	m_pModelBase->property._color = Color(0, 0, 0, 1);
	m_pModelBase->SetTexture(m_pTexture.get());

	RegisterScene(new SceneGameover());
	return true;
}

void SceneBattle::Finalize(){
	m_pModelBase->Finalize();
	return;
}

bool SceneBattle::Updater(){
	if (GameController::GetMouse().IsLeftButtonTrigger()){
		ChangeScene("Gameover", LoadState::eUnuse, LoadWaitState::eUnuse);
	}
	return true;
}

void SceneBattle::Render(){
	m_pTitleView->Render();
	m_pModelBase->Render(m_pShaderBase.get());
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