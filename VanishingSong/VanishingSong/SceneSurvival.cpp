#include "SceneSurvival.h"
#include <iostream>
#include"PixelShader.h"
#include <GameController.h>

using namespace aetherClass;
namespace{
	const float kDefaultScaleTime = 1.0f;
}
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

	// プレイヤーの初期化
	m_pPlayer = std::make_unique<Player>();
	m_pPlayer->mInitialize();

	auto view = m_pPlayer->mGetView();

	m_penemyGround = std::make_shared<EnemyGround>();
	m_penemyGround->mInitialize(view);

	// アクションコマンドの初期化
	m_pActionBoard = std::make_unique<ActionBoard>();
	m_pActionBoard->mInitialize();

	// オーダーリストの初期化
	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize();

	m_pFieldArea = std::make_unique<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	return true;
}

// 解放処理
void SceneSurvival::Finalize(){

	if (m_pActionBoard){
		m_pActionBoard.release();
		m_pActionBoard = nullptr;
	}

	if (m_pOrderList){
		m_pOrderList.release();
		m_pOrderList = nullptr;
	}

	if (m_pFieldArea){
		m_pFieldArea.release();
		m_pFieldArea = nullptr;
	}

	if (m_pPlayer){
		m_pPlayer.release();
		m_pPlayer = nullptr;
	}

	return;
}

// 更新処理
bool SceneSurvival::Updater(){
	m_pPlayer->mUpdate(kDefaultScaleTime);

	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	m_pOrderList->mUpdate(kDefaultScaleTime);
	return true;
}

void SceneSurvival::Render(){

	auto view = m_pPlayer->mGetView();
	view->Render();
	m_pPlayer->mRender(m_pixelShader.get(), m_pixelShader.get());
	
	m_penemyGround->mRender(m_pixelShader.get(),m_pixelShader.get());
	m_pFieldArea->mRender(m_pixelShader.get());
	return;
}

void SceneSurvival::UIRender(){

	m_pActionBoard->mRender(m_pixelShader.get());
	m_pOrderList->mRender(m_pixelShader.get());
	return;
}

bool SceneSurvival::TransitionIn(){

	return kTransitionEnd;
}

bool SceneSurvival::TransitionOut(){

	return kTransitionEnd;
}