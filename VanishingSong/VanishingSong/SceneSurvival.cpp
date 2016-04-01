#include "SceneSurvival.h"
#include <iostream>
#include"PixelShader.h"

using namespace aetherClass;
SceneSurvival::SceneSurvival():
GameScene("Game", GetManager()) //Sceneごとの名前を設定
{
}


SceneSurvival::~SceneSurvival()
{
}

bool SceneSurvival::Initialize(){

	m_camera = std::make_shared<ViewCamera>();
	m_camera->property._translation = Vector3(0, -0, 0);
	m_camera->property._rotation = Vector3(0.0f, 0.0f, 0.0f);


	// シェーダーの詳細情報の設定
	ShaderDesc textureDesc;

	textureDesc._vertex._entryName = "vs_main";
	textureDesc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	textureDesc._pixel._entryName = "ps_main";
	textureDesc._pixel._srcFile = L"Shader/BasicColor.hlsl";

	// ピクセルシェーダーの作成
	m_pixelShader = std::make_shared<PixelShader>();
	m_pixelShader->Initialize(textureDesc, ShaderType::eVertex | ShaderType::ePixel);

	m_penemyGround = std::make_shared<EnemyGround>();
	m_penemyGround->mInitialize(m_camera.get());


	return true;
}

void SceneSurvival::Finalize(){

	return;
}

bool SceneSurvival::Updater(){

	return true;
}

void SceneSurvival::Render(){
	m_camera->Render();
	m_penemyGround->mRender(m_pixelShader.get(),m_pixelShader.get());


	return;
}

void SceneSurvival::UIRender(){

	return;
}

bool SceneSurvival::TransitionIn(){

	return kTransitionEnd;
}

bool SceneSurvival::TransitionOut(){

	return kTransitionEnd;
}