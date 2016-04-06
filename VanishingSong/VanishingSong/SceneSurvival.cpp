#include "SceneSurvival.h"
#include <iostream>
#include"PixelShader.h"

using namespace aetherClass;
SceneSurvival::SceneSurvival():
GameScene("Survival", GetManager()) //Sceneごとの名前を設定
{
}


SceneSurvival::~SceneSurvival()
{
}

bool SceneSurvival::Initialize(){

	// シェーダーの詳細情報の設定
	ShaderDesc textureDesc;

	textureDesc._vertex._entryName = "vs_main";
	textureDesc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	textureDesc._pixel._entryName = "ps_main";
	textureDesc._pixel._srcFile = L"Shader/BasicColor.hlsl";

	// ピクセルシェーダーの作成
	m_pixelShader = std::make_shared<PixelShader>();
	m_pixelShader->Initialize(textureDesc, ShaderType::eVertex | ShaderType::ePixel);

	m_pPlayer = std::make_unique<Player>();
	m_pPlayer->mInitialize();

	m_penemyGround = std::make_shared<EnemyGround>();
	m_penemyGround->mInitialize(&m_pPlayer->mGetView());

	

	return true;
}

void SceneSurvival::Finalize(){

	return;
}

bool SceneSurvival::Updater(){
	m_pPlayer->mUpdate(1);
	return true;
}

void SceneSurvival::Render(){

	m_penemyGround->mRender(m_pixelShader.get(),m_pixelShader.get());
	m_pPlayer->mRender(m_pixelShader.get(), m_pixelShader.get());


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