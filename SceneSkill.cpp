#include "SceneSkill.h"
#include <iostream>
#include<PixelShader.h>
#include<Rectangle3D.h>
#include <GameController.h>

#include "SceneBattle.h"

using namespace aetherClass;

SceneSkill::SceneSkill():
GameScene("SkillSelection", GetManager())
{
}

SceneSkill::~SceneSkill()
{
}

bool SceneSkill::Initialize(){
	m_pTitleView = std::make_unique<ViewCamera>();
	m_pTitleView->property._translation = Vector3(0, 0, -3);

	m_pTexture = std::make_unique<Texture>();
	m_pTexture->Load("Texture/SkillImage.png");

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

	RegisterScene(new SceneBattle());
	return true;
}

void SceneSkill::Finalize(){
	m_pModelBase->Finalize();
	return;
}

bool SceneSkill::Updater(){
	if (GameController::GetMouse().IsLeftButtonTrigger()){
		ChangeScene("Battle", LoadState::eUnuse, LoadWaitState::eUnuse);
	}
	return true;
}

void SceneSkill::Render(){
	m_pTitleView->Render();
	m_pModelBase->Render(m_pShaderBase.get());
	return;
}

void SceneSkill::UIRender(){
	return;
}

bool SceneSkill::TransitionIn(){
	return kTransitionEnd;
}

bool SceneSkill::TransitionOut(){
	return kTransitionEnd;
}