#include "SceneSurvival.h"
#include <iostream>
#include"PixelShader.h"
#include<GameController.h>

#include"ActionBoard.h"
#include"OrderList.h"
#include"FieldArea.h"


using namespace aetherClass;
SceneSurvival::SceneSurvival():
GameScene("Survival", GetManager()) //Sceneごとの名前を設定
{
}


SceneSurvival::~SceneSurvival()
{
}

ActionBoard* g_actionBoard;
OrderList* g_orderList;
FieldArea* g_FieldArea;

bool SceneSurvival::Initialize(){

	m_camera = std::make_shared<ViewCamera>();
	m_camera->property._translation = Vector3(0, 0, -20);
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

	//m_pPlayer = std::make_unique<Player>();
	//m_pPlayer->mInitialize(m_camera.get());

	g_actionBoard = new ActionBoard();
	g_actionBoard->mInitialize();

	g_orderList = new OrderList();
	g_orderList->mInitialize();

	g_FieldArea = new FieldArea ();
	g_FieldArea->mInitialize();
	g_FieldArea->mSetCamera(m_camera.get());

	return true;
}

void SceneSurvival::Finalize(){
	delete g_actionBoard;
	delete g_FieldArea;
	delete g_orderList;

	return;
}

bool SceneSurvival::Updater(){
//	m_pPlayer->mUpdate(1);
	auto actionCommand = g_actionBoard->mSelectType();
	if (actionCommand){
		g_orderList->mAddOrder(actionCommand);
	}
	g_orderList->mUpdate(1);
	

	return true;
}

void SceneSurvival::Render(){
	m_camera->Render();
	m_penemyGround->mRender(m_pixelShader.get(),m_pixelShader.get());

	g_FieldArea->mRender(m_pixelShader.get());
	//m_pPlayer->mRender(m_pixelShader.get(), m_pixelShader.get());

	return;
}

void SceneSurvival::UIRender(){
	g_actionBoard->mRender(m_pixelShader.get());
	g_orderList->mRender(m_pixelShader.get());

	return;
}

bool SceneSurvival::TransitionIn(){

	return kTransitionEnd;
}

bool SceneSurvival::TransitionOut(){

	return kTransitionEnd;
}