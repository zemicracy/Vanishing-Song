#include "SceneTitle.h"
#include <iostream>
#include<PixelShader.h>
#include<Rectangle3D.h>
#include <GameController.h>

#include "SceneSkill.h"

using namespace aetherClass;

SceneTitle::SceneTitle():
GameScene("Title",GetManager())
{

}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Initialize(){
	//カメラ
	m_pTitleView = std::make_unique<ViewCamera>();
	m_pTitleView->property._translation = Vector3(0, 0, -1);

	//テクスチャ読み込み
	m_pTexture = std::make_unique<Texture>();
	m_pTexture->Load("Texture/TitleImage.png");

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
	
	//次のシーン
	RegisterScene(new SceneSkill());
	
	return true;
}

void SceneTitle::Finalize(){
	m_pModelBase->Finalize();
	return;
}

bool SceneTitle::Updater(){
	if(GameController::GetMouse().IsLeftButtonTrigger()){
		ChangeScene("SkillSelection", LoadState::eUnuse, LoadWaitState::eUnuse);

	}
	return true;
}

void SceneTitle::Render(){
	m_pTitleView->Render();
	m_pModelBase->Render(m_pShaderBase.get());
	return;
}

void SceneTitle::UIRender(){
	return;
}

bool SceneTitle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneTitle::TransitionOut(){
	return kTransitionEnd;
}