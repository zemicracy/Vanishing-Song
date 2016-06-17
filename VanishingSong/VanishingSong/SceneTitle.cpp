#include "SceneTitle.h"
#include <iostream>
#include <PixelShader.h>
#include <Rectangle2D.h>
#include <GameController.h>
#include <Singleton.h>
#include <ModelUtility.h>
#include "ResourceManager.h"
#include "Debug.h"
#include "Const.h"
#include "ResourceManager.h"
#include "SceneGame.h"
#include"SceneCregit.h"
using namespace aetherClass;
using namespace aetherFunction;
namespace{
	const bool kPleaseClick = false;
	const bool kMenuSelect = true;
	const std::string kExit = "Exit";
	const bool kShutdown = false;
	const int kSkipNumber = 3;
}
const std::string SceneTitle::Name = "Title";
SceneTitle::SceneTitle() :
GameScene(Name, GetManager())
{
	m_pLogo = nullptr;
	m_pLogoTexture = nullptr;
	m_pMenuTexture = nullptr;
	m_pPushTexture = nullptr;
	m_pMenu = nullptr;
	m_pCursor = nullptr;
	m_nowSelectMode = NULL;
}

SceneTitle::~SceneTitle()
{
	m_pLogo = nullptr;
	m_pLogoTexture = nullptr;
	m_pMenuTexture = nullptr;
	m_pPushTexture = nullptr;
	m_pMenu = nullptr;
	m_pCursor = nullptr;
	m_nowSelectMode = NULL;
}

bool SceneTitle::Initialize(){
	_heapmin();
	//次のシーン
	RegisterScene(new SceneGame());
	RegisterScene(new SceneCregit());
	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eRed, "Model\\Player", "Model\\Player\\red");

	
	// テクスチャの初期化
	m_pLogoTexture = std::make_shared<Texture>();
	m_pLogoTexture->Load("Texture\\Title\\Logo.png");

	m_pMenuTexture = std::make_shared<Texture>();
	m_pMenuTexture->Load("Texture\\Title\\Select.png");

	m_pPushTexture = std::make_shared<Texture>();
	m_pPushTexture->Load("Texture\\Title\\PushStart.png");
	
	// ロゴの初期化
	m_pLogo = std::make_unique<Rectangle2D>();
	m_pLogo->Initialize();
	m_pLogo->SetTexture(m_pLogoTexture.get());
	m_pLogo->property._transform._scale = Vector3(600, 500, 0);

	const float logoX = (kWindowWidth / 2) - (m_pLogo->property._transform._scale._x / 2);
	const float logoY = (kWindowHeight / 2) - (m_pLogo->property._transform._scale._y) + 100;
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
	m_pCursor->property._transform._translation = m_pMenu->property._transform._translation + Vector3(0,35,0);

	const float cursorPosition = m_pCursor->property._transform._translation._y;
	const float cursorSize = m_pCursor->property._transform._scale._y;

	for (int i = 0; i < m_cursorArray.size(); ++i){
		m_cursorArray[i]._cursorY = cursorPosition + (i * (35 + cursorSize));
		m_cursorArray[i]._modeNumber = eNextMode::eNull + (i + 1);
	}
	
	m_pushState = false;
	m_alphaState = false;
	m_nowSelectMode = NULL;
	m_nowCursor = NULL;
	
	m_pSkybox = std::make_unique<Skybox>();
	m_pSkybox->Initialize();
	m_pSkybox->SetCamera(&m_view);
	m_pSkybox->SetTexture(ResourceManager::mGetInstance().GetTexture("skybox").get());

	m_pSound = std::make_shared<GameSound>();
	m_pSound->Load("Sound\\Result\\title.wav");
	m_pSound->SetValume(-100);

	_heapmin();
	return true;
}

void SceneTitle::Finalize(){
	_heapmin();
	if (m_pSound){
		m_pSound.reset();
		m_pSound = nullptr;
	}

	if (m_pLogo){
		m_pLogo->Finalize();
		m_pLogo.reset();
		m_pLogo = nullptr;
	}

	if (m_pCursor){
		m_pCursor->Finalize();
		m_pCursor.reset();
		m_pCursor = nullptr;
	}

	if (m_pMenu){
		m_pMenu->Finalize();
		m_pMenu.reset();
		m_pMenu = nullptr;
	}

	if (m_pLogoTexture){
		m_pLogoTexture.reset();
		m_pLogoTexture = nullptr;
	}

	if (m_pMenuTexture){
		m_pMenuTexture.reset();
		m_pMenuTexture = nullptr;
	}

	if (m_pPushTexture){
		m_pPushTexture.reset();
		m_pPushTexture = nullptr;
	}

	if (m_pSkybox){
		m_pSkybox->Finalize();
		m_pSkybox.reset();
		m_pSkybox = nullptr;
	}

	m_pushState = false;

	m_alphaState = false;
	m_nowSelectMode = NULL;

	// NULLで初期化
	m_cursorArray.fill(ModeSelect(NULL));

	_heapmin();
	return;
}

//
bool SceneTitle::Updater(){
	m_pSound->PlayToLoop();

	const bool isStart = GameController::GetKey().KeyDownTrigger(VK_SPACE) || GameController::GetJoypad().ButtonPress(eJoyButton::eStart);
	const bool isReturn = GameController::GetKey().KeyDownTrigger(VK_RETURN) || GameController::GetJoypad().ButtonPress(eJoyButton::eB);
	std::pair<bool, bool> UpOrDown;
	UpOrDown.first = GameController::GetKey().KeyDownTrigger(VK_UP) || GameController::GetJoypad().ButtonPress(eJoyButton::eUp);
	UpOrDown.second = GameController::GetKey().KeyDownTrigger(VK_DOWN) || GameController::GetJoypad().ButtonPress(eJoyButton::eDown);
	
	mCursorState(isStart);
	bool isUpdate = mMenuSelectState(isReturn,UpOrDown);

	if (kCharaDebug){
		if (GameController::GetKey().KeyDownTrigger('T')){
			GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eFirstStage);
		}
	}
	// まだ続けるか？
	if (!isUpdate)
	{
		return kShutdown;
	}
	return true;
}

void SceneTitle::Render(){
	m_view.Render();
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	m_pSkybox->Render(shaderHash["texture"].get());
	
	return;
}

void SceneTitle::UIRender(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	m_pLogo->Render(shaderHash["transparent"].get());
	m_pMenu->Render(shaderHash["transparent"].get());

	if (m_pushState == kMenuSelect){
		m_pCursor->Render(shaderHash["color"].get());
	}
	GameManager::mGetInstance().mfadeManager().mRender(shaderHash["color"].get());
	return;
}

bool SceneTitle::TransitionIn(){
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		return kTransitionning;
	}
	return kTransitionEnd;
}

//
bool SceneTitle::TransitionOut(){
	if (!GameManager::mGetInstance().mfadeManager().Out(1)){
		return kTransitionning;
	}
	return kTransitionEnd;
}


void SceneTitle::mChangeSelect(const bool isUp, const bool isDown){
	const float cursorSize = m_pCursor->property._transform._scale._y;
	if (isUp){
		m_nowCursor -= 1;
	}
	else if (isDown){
		m_nowCursor += 1;
	}

	if (m_nowCursor > m_cursorArray.size() - 1){
		m_nowCursor = 0;
	}
	else if (m_nowCursor < 0){
		m_nowCursor = m_cursorArray.size() - 1;
	}

	m_pCursor->property._transform._translation._y = m_cursorArray[m_nowCursor]._cursorY;
	m_nowSelectMode = m_cursorArray[m_nowCursor]._modeNumber;

}


void SceneTitle::mCursorState(const bool isStart){
	if (m_pushState != kPleaseClick)return;

	if (isStart){
		m_pMenu->SetTexture(m_pMenuTexture.get());
		m_pushState = kMenuSelect;
		m_pMenu->property._color._alpha = 1;
	}

	// 増減の選択
	if (m_pMenu->property._color._alpha >= 1.1f){
		m_alphaState = false;
	}
	else if(m_pMenu->property._color._alpha <= -0.1f){
		m_alphaState = true;
	}

	// 増減の実行
	if (m_alphaState){
		m_pMenu->property._color._alpha += 0.01;
	}
	else{
		m_pMenu->property._color._alpha -= 0.01;
	}

}

//
bool SceneTitle::mMenuSelectState(const bool isReturn, const  std::pair<bool, bool> UpOrDown){
	if (m_pushState != kMenuSelect)return true;

	mChangeSelect(UpOrDown.first, UpOrDown.second);

	if (isReturn){
		SceneInfo nextState = mGetGameMode(m_nowSelectMode);
		// Exit以外が来たらシーンの遷移を開始
		if (nextState._nextSceneName != kExit){
			// シーンの遷移
			ChangeScene(nextState._nextSceneName, LoadState::eUse);
		}
		else{
			// 終了
			return kShutdown;
		}
	}
	return true;
}


SceneTitle::SceneInfo SceneTitle::mGetGameMode(const int index){
	SceneInfo info;
	switch (index){
	case eNextMode::eStart:
		info._nextSceneName = SceneGame::Name;
		break;
	case eNextMode::eCredit:
		// ここはならすべて終了
		info._nextSceneName = SceneCregit::Name;
		break;
	case eNextMode::eNull:
	case eNextMode::eExit:
		info._nextSceneName = kExit;
		break;
	}
	return info;
}