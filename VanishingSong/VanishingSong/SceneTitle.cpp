#include "SceneTitle.h"
#include <iostream>
#include <PixelShader.h>
#include <Rectangle2D.h>
#include <GameController.h>
#include <Singleton.h>
#include <ModelUtility.h>
#include "Debug.h"
#include "Const.h"
#include "ResourceManager.h"
#include "SceneGame.h"
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

	// �e�N�X�`���̏�����
	m_pLogoTexture = std::make_shared<Texture>();
	m_pLogoTexture->Load("Texture\\Title\\TitleImage.png");

	m_pMenuTexture = std::make_shared<Texture>();
	m_pMenuTexture->Load("Texture\\Title\\SelectMode.png");

	m_pPushTexture = std::make_shared<Texture>();
	m_pPushTexture->Load("Texture\\Title\\TitleImage.png");
	
	// ���S�̏�����
	m_pLogo = std::make_unique<Rectangle2D>();
	m_pLogo->Initialize();
	m_pLogo->SetTexture(m_pLogoTexture.get());
	m_pLogo->property._transform._scale = Vector3(600, 300, 0);

	const float logoX = (kWindowWidth / 2) - (m_pLogo->property._transform._scale._x / 2);
	const float logoY = (kWindowHeight / 2) - (m_pLogo->property._transform._scale._y);
	m_pLogo->property._transform._translation = Vector3(logoX, logoY, 0);
	m_pLogo->property._color = Color(0, 0, 0, 1);

	// ���j���[�̏�����
	m_pMenu = std::make_unique<Rectangle2D>();
	m_pMenu->Initialize();
	m_pMenu->SetTexture(m_pPushTexture.get());
	m_pMenu->property._transform._scale = Vector3(400, 300, 0);

	const float menuX = (kWindowWidth / 2) - (m_pMenu->property._transform._scale._x / 2);
	const float menuY = (kWindowHeight / 2) +(kWindowHeight/20);
	m_pMenu->property._transform._translation = Vector3(menuX, menuY, 0);
	m_pMenu->property._color = Color(0, 0, 0, 1);

	// �J�[�\���̏�����
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
	//���̃V�[��
	RegisterScene(new SceneGame());

	m_pushState = false;
	m_alphaState = false;
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

	m_pushState = false;

	m_alphaState = false;
	m_nowSelectMode = NULL;

	// NULL�ŏ�����
	m_cursorArray.fill(ModeSelect(NULL));
	return;
}

bool SceneTitle::Updater(){
	
	mClickState();
	bool isUpdate = mMenuSelectState();
	// �܂������邩�H
	if (!isUpdate)
	{
		return kShutdown;
	}
	return true;
}

void SceneTitle::Render(){

	
	return;
}

void SceneTitle::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pLogo->Render(shaderHash["transparent"].get());
	m_pMenu->Render(shaderHash["transparent"].get());

	if (m_pushState == kMenuSelect){

		m_pCursor->Render(shaderHash["color"].get());
	}
	return;
}

bool SceneTitle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneTitle::TransitionOut(){
	return kTransitionEnd;
}


void SceneTitle::mChangeSelect(Vector2 mouse){
	const float cursorSize = m_pCursor->property._transform._scale._y;
	for (int i = 0; i < m_cursorArray.size();++i){
		if (i == kSkipNumber)continue;
		if (m_cursorArray[i]._cursorY<mouse._y&&m_cursorArray[i]._cursorY + cursorSize>mouse._y){
			m_pCursor->property._transform._translation._y = m_cursorArray[i]._cursorY;
			m_nowSelectMode = m_cursorArray[i]._modeNumber;
		}
	}
}


void SceneTitle::mClickState(){
	if (m_pushState != kPleaseClick)return;

	if (GameController::GetMouse().IsLeftButtonTrigger()){
		m_pMenu->SetTexture(m_pMenuTexture.get());
		m_pushState = kMenuSelect;
	}

	// �����̑I��
	if (m_pMenu->property._color._alpha >= 1.1f){
		m_alphaState = false;
	}
	else if(m_pMenu->property._color._alpha <= -0.1f){
		m_alphaState = true;
	}

	// �����̎��s
	if (m_alphaState){
		m_pMenu->property._color._alpha += 0.01;
	}
	else{
		m_pMenu->property._color._alpha -= 0.01;
	}

}


bool SceneTitle::mMenuSelectState(){
	if (m_pushState != kMenuSelect)return true;

	m_pMenu->property._color._alpha = 1;
	// �J�[�\���̈ړ�����
	Vector2 mouse = GameController::GetMouse().GetMousePosition();
	DirectXEntity directX;
	Vector2 screenSize = GetWindowSize(directX.GetWindowHandle(kWindowName));
	//�E�B���h�E�T�C�Y����
	mouse._x = (mouse._x / (screenSize._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screenSize._x;
	mouse._y = (mouse._y / (screenSize._y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CXDLGFRAME)))* screenSize._y;

	// �J�[�\���̐؂�ւ�
	mChangeSelect(mouse);

	// �J�[�\���̏�ŃN���b�N������
	Vector3 size = m_pCursor->property._transform._scale;
	Vector3 origin = m_pCursor->property._transform._translation;
	if (mouse._x > origin._x&&mouse._x <= origin._x + size._x&&
		mouse._y > origin._y&&mouse._y <= origin._y + size._y){

		if (GameController::GetMouse().IsLeftButtonTrigger()){
			SceneInfo nextState = mGetGameMode(m_nowSelectMode);
			// Exit�ȊO��������V�[���̑J�ڂ��J�n
			if (nextState._nextSceneName != kExit){

				// �Q�[�����[�h�̐ݒ�
				Singleton<GameManager>::GetInstance().mGameMode(nextState._mode);
				// �V�[���̑J��
				ChangeScene(nextState._nextSceneName, LoadState::eUse);
			}else{
				// �I��
				return kShutdown;
			}
		}
	}
}


SceneTitle::SceneInfo SceneTitle::mGetGameMode(const int index){
	SceneInfo info;
	switch (index){
	case eNextMode::eSurvival:
		info._mode = GameManager::eGameMode::eGame;
		info._nextSceneName = SceneGame::Name;
		break;

		// �����͂Ȃ炷�ׂďI��
	case eNextMode::eNull:
	case eNextMode::eExit:
		info._mode = GameManager::eGameMode::eNull;
		info._nextSceneName = kExit;
		break;
	}
	return info;
}