#include "SceneTitle.h"
#include <iostream>
#include <PixelShader.h>
#include <Rectangle2D.h>
#include <GameController.h>
#include <Singleton.h>
#include <ModelUtility.h>
#include "Debug.h"
#include "SceneSkill.h"
#include "Const.h"
using namespace aetherClass;
using namespace aetherFunction;
namespace{
	const bool kPleaseClick = false;
	const bool kMenuSelect = true;
}
const std::string SceneTitle::Name = "Title";
SceneTitle::SceneTitle() :
GameScene(Name, GetManager())
{
	m_pLogo = nullptr;
	m_pTextureShader = nullptr;
	m_pColorShader = nullptr;
	m_pLogoTexture = nullptr;
	m_pMenuTexture = nullptr;
	m_pPushTexture = nullptr;
	m_pMenu = nullptr;
	m_pCursor = nullptr;
	m_isExit = false;
	m_nowSelectMode = NULL;
}

SceneTitle::~SceneTitle()
{
	m_pLogo = nullptr;
	m_pTextureShader = nullptr;
	m_pColorShader = nullptr;
	m_pLogoTexture = nullptr;
	m_pMenuTexture = nullptr;
	m_pPushTexture = nullptr;
	m_pMenu = nullptr;
	m_pCursor = nullptr;
	m_isExit = false;
	m_nowSelectMode = NULL;
}

bool SceneTitle::Initialize(){

	// シェーダーの初期化
	ShaderDesc desc;
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader\\VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader\\Texture.hlsl";
	
	m_pTextureShader = std::make_shared<PixelShader>();
	m_pTextureShader->Initialize(desc, ShaderType::eVertex | ShaderType::ePixel);

	desc._pixel._srcFile = L"Shader\\BasicColor.hlsl";

	m_pColorShader = std::make_shared<PixelShader>();
	m_pColorShader->Initialize(desc, ShaderType::eVertex | ShaderType::ePixel);
	// テクスチャの初期化
	m_pLogoTexture = std::make_shared<Texture>();
	m_pLogoTexture->Load("Texture\\TitleImage.png");

	m_pMenuTexture = std::make_shared<Texture>();
	m_pMenuTexture->Load("Texture\\space.jpg");

	m_pPushTexture = std::make_shared<Texture>();
	m_pPushTexture->Load("Texture\\TitleImage.png");
	
	// ロゴの初期化
	m_pLogo = std::make_unique<Rectangle2D>();
	m_pLogo->Initialize();
	m_pLogo->SetTexture(m_pLogoTexture.get());
	m_pLogo->property._transform._scale = Vector3(600, 300, 0);

	const float logoX = (kWindowWidth / 2) - (m_pLogo->property._transform._scale._x / 2);
	const float logoY = (kWindowHeight / 2) - (m_pLogo->property._transform._scale._y);
	m_pLogo->property._transform._translation = Vector3(logoX, logoY, 0);
	m_pLogo->property._color = Color(0, 0, 0, 1);

	// メニューの初期化
	m_pMenu = std::make_unique<Rectangle2D>();
	m_pMenu->Initialize();
	m_pMenu->SetTexture(m_pPushTexture.get());
	m_pMenu->property._transform._scale = Vector3(400, 300, 0);

	const float menuX = (kWindowWidth / 2) - (m_pMenu->property._transform._scale._x / 2);
	const float menuY = (kWindowHeight / 2) +(kWindowHeight/20);
	m_pMenu->property._transform._translation = Vector3(menuX, menuY, 0);
	m_pMenu->property._color = Color(0, 0, 0, 1);

	// カーソルの初期化
	m_pCursor = std::make_unique<Rectangle2D>();
	m_pCursor->Initialize();
	m_pCursor->property._color = Color(1, 1, 1, 0.3);
	m_pCursor->property._transform._scale = Vector3(400, 50, 0);
	m_pCursor->property._transform._translation = m_pMenu->property._transform._translation + Vector3(0,5,0);

	const float cursorPosition = m_pCursor->property._transform._translation._y;
	const float cursorSize = m_pCursor->property._transform._scale._y;

	for (int i = 0; i < m_cursorArray.size(); ++i){
		m_cursorArray[i]._cursorY = cursorPosition + (i * (10 + cursorSize));
		m_cursorArray[i]._modeNumber = eNextMode::eNull + (i + 1);
	}
	//次のシーン
	RegisterScene(new SceneSkill());

	m_isExit = false;
	m_pushState = false;
	m_nowSelectMode = NULL;
	return true;
}

void SceneTitle::Finalize(){
	
	if (m_pLogo){
		m_pLogo->Finalize();
		m_pLogo.release();
		m_pLogo = nullptr;
	}

	if (m_pCursor){
		m_pCursor->Finalize();
		m_pCursor.release();
		m_pCursor = nullptr;
	}

	if (m_pMenu){
		m_pMenu->Finalize();
		m_pMenu.release();
		m_pMenu = nullptr;
	}

	if (m_pTextureShader)
	{
		m_pTextureShader->Finalize();
		m_pTextureShader = nullptr;
	}

	if (m_pColorShader)
	{
		m_pColorShader->Finalize();
		m_pColorShader = nullptr;

	}

	m_pushState = false;
	m_isExit = false;
	m_nowSelectMode = NULL;
	return;
}

bool SceneTitle::Updater(){
	if(GameController::GetMouse().IsLeftButtonTrigger()){

		// 前回が押してください状態なら、画像を切り替える
		if (m_pushState == kPleaseClick){
			m_pMenu->SetTexture(m_pMenuTexture.get());
			m_pushState = true;
		}
		else{
			if (m_nowSelectMode != SceneTitle::eNextMode::eNull){
				// 選択したシーンに行く
			}
		}
	}



	// それぞれの更新処理
	if (m_pushState == kMenuSelect){
		m_pCursor->Render(m_pColorShader.get());
		// カーソルの移動処理
		Vector2 mouse = GameController::GetMouse().GetMousePosition();
		DirectXEntity directX;
		Vector2 screenSize = GetWindowSize(directX.GetWindowHandle(kWindowName));
		//ウィンドウサイズ調整
		mouse._x = (mouse._x / (screenSize._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screenSize._x;
		mouse._y = (mouse._y / (screenSize._y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CXDLGFRAME)))* screenSize._y;

		// カーソルの切り替え
		ChangeSelect(mouse);
	}
	return true;
}

void SceneTitle::Render(){

	
	return;
}

void SceneTitle::UIRender(){
	m_pLogo->Render(m_pTextureShader.get());
	m_pMenu->Render(m_pTextureShader.get());

	if (m_pushState == kMenuSelect){
		m_pCursor->Render(m_pColorShader.get());
	}
	return;
}

bool SceneTitle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneTitle::TransitionOut(){
	return kTransitionEnd;
}

GameManager::eGameMode SceneTitle::mGetGameMode(const int index){
	switch (index){
		case eNextMode::eNull:
		break;
		case eNextMode::eSurvival:
			break;
		case eNextMode::eBattle:
			break;
		case eNextMode::ePractice:
			break;
		case eNextMode::eTutorial:
			break;
		case eNextMode::eExit:
			m_isExit = true;
			break;
	}
	return GameManager::eGameMode::eNull;
}

void SceneTitle::ChangeSelect(Vector2 mouse){
	const float cursorSize = m_pCursor->property._transform._scale._y;
	for (auto index : m_cursorArray){
		if (index._cursorY<mouse._y&&index._cursorY + cursorSize>mouse._y){
			m_pCursor->property._transform._translation._y = index._cursorY;
			m_nowSelectMode = index._modeNumber;
		}
	}
}