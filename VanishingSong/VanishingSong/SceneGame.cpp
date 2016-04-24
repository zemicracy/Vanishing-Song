#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
using namespace aetherClass;
namespace{
	const float kDefaultScaleTime = 100.0f; 
	const float kCommandTimeScale = 1.0f;
	const bool kError = false;
}
const std::string SceneGame::Name = "Game";

SceneGame::SceneGame():
GameScene(Name, GetManager()) //Scene���Ƃ̖��O��ݒ�
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}

bool SceneGame::Initialize(){

	bool result = false;

	result = mInitalizeShader();
	if (!result){
		Debug::mErrorPrint("�V�F�[�_�[���������s", __FILE__, __LINE__);
		return false;
	}

	
	// �v���C���[�̏�����
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->mInitialize();

	auto view = m_pPlayer->mGetView();

	m_penemyGround = std::make_shared<EnemyGround>();
	m_penemyGround->mInitialize(view);

	// �A�N�V�����R�}���h�̏�����
	m_pActionBoard = std::make_unique<ActionBoard>();
	m_pActionBoard->mInitialize(GameManager::eSkillType::eExAttack);

	// �I�[�_�[���X�g�̏�����
	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize();

	// �X�e�[�W�I�u�W�F�N�g
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	// �t�F�[�h�C���E�A�E�g���s��
	m_pFadeObject = std::make_unique<FadeManager>();
	// �Q�[�����[�h�̎擾
	// ���̃V�[���ɗ��Ă��鎞�_�ŁA�T�o�C�o�����A�v���N�e�B�X���I������Ă���
	m_nowMode = Singleton<GameManager>::GetInstance().mGameMode(); 
	m_nowMode = GameManager::eGameMode::eSurvaival;
	mInitializeMode(m_nowMode);
	m_dayTime = 0.0f;
	
	m_pResultBord = std::make_unique<ResultBord>();
	m_pResultBord->mInitialize(m_nowMode);

	RegisterScene(new SceneTitle());

	m_pCollideManager = std::make_unique<CollideManager>();

	m_pCollideManager->mInitialize(m_pPlayer, nullptr, m_pFieldArea);
	// �Q�[���̏�Ԃ�o�^
	m_gameState = eState::eRun;

	return true;
}

// �������
// �S�Ẳ��
void SceneGame::Finalize(){
	if (m_pResultBord){
		m_pResultBord->mFinalize();
		m_pResultBord.release();
		m_pResultBord = nullptr;
	}

	if (m_pActionBoard){
		m_pActionBoard.release();
		m_pActionBoard = nullptr;
	}

	if (m_pOrderList){
		m_pOrderList.release();
		m_pOrderList = nullptr;
	}

	if (m_pFieldArea){
		m_pFieldArea.reset();
		m_pFieldArea = nullptr;
	}

	if (m_pPlayer){
		m_pPlayer->mFinalize();
		m_pPlayer.reset();
		m_pPlayer = nullptr;
	}

	if (m_penemyGround){
		m_penemyGround->mFinalize();
		m_penemyGround.reset();
		m_penemyGround = nullptr;
	}
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;
	return;
}

// �X�V����
bool SceneGame::Updater(){
	if (m_gameState == eState::eFadeIn || m_gameState == eState::eFadeOut){
		bool result = mFadeState(m_gameState);
		if (!result){
			return true;
		}
	}
	if (m_gameState == eState::eExit){
		// �I���Ȃ�^�C�g���ɖ߂�
		ChangeScene(SceneTitle::Name, LoadState::eUse, LoadWaitState::eUnuse);
	}

	bool result = false;
	// �����̎擾
	m_dayTime += GameClock::GetDeltaTime();
	// �T�o�C�o�����[�h���̍X�V����
	result = this->mSurvivalMainUpdate(kDefaultScaleTime, m_dayTime);
	if (!result){
		return kError;
	}

	// �Ճ��N�e�B���[�h���̍X�V����
	result = this->mPracticeMainUpdate(kDefaultScaleTime, m_dayTime);
	if (!result){
		return kError;
	}

	m_pCollideManager->mUpdate();
	return true;
}

void SceneGame::Render(){
	this->mSurvivalRender();
	this->mPracticeRender();
	
	return;
}

void SceneGame::UIRender(){
	this->mSurvivalUIRender();
	this->mPracticeUIRender();
	mShowResult(m_nowDay, m_pixelShader.get(), m_pixelShader.get());

	if (m_gameState == eState::eFadeIn || m_gameState == eState::eFadeOut){
		m_pFadeObject->mRedner(m_pixelShader.get());
	}
	return;
}

bool SceneGame::TransitionIn(){

	return kTransitionEnd;
}

bool SceneGame::TransitionOut(){

	return kTransitionEnd;
}

bool SceneGame::mInitalizeShader(){
	// �V�F�[�_�[�̏ڍ׏��̐ݒ�
	ShaderDesc textureDesc;
	bool result = false;
	textureDesc._vertex._entryName = "vs_main";
	textureDesc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	textureDesc._pixel._entryName = "ps_main";
	textureDesc._pixel._srcFile = L"Shader/BasicColor.hlsl";

	// �s�N�Z���V�F�[�_�[�̍쐬
	m_pixelShader = std::make_shared<PixelShader>();
	result = m_pixelShader->Initialize(textureDesc, ShaderType::eVertex | ShaderType::ePixel);
	if (!result){
		return false;
	}

	return true;
}

/*
	���ꂼ��̃��[�h�̓Ǝ��̏�����
*/
bool SceneGame::mInitializeMode(GameManager::eGameMode mode){
	switch (mode)
	{
	case GameManager::eGameMode::eSurvaival:
		m_nowDay = GameManager::eDay::e1day;
		mRegisterDay();
		Debug::mPrint("�T�o�C�o�����[�h");

		break;
	case GameManager::eGameMode::ePractice:
		Debug::mPrint("�v���N�e�B�X���[�h");
		break;
	case GameManager::eGameMode::eNull:
		assert(!"�s���Ȓl�����o���܂��� :Part SG");
		return false;
	}
	return true;
}


/*
	�T�o�C�o�����[�h���̍X�V����
*/
bool SceneGame::mSurvivalMainUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::eSurvaival || m_gameState != eState::eRun)return true;
	if (GameController::GetKey().KeyDownTrigger(VK_RETURN)){
		m_gameState = eState::eResult;
	}

	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	m_pPlayer->mCommand(m_pOrderList->mGetActionCommand(), kDefaultScaleTime);
	m_pPlayer->mUpdate(kDefaultScaleTime);

	m_pActionBoard->mUpdate(kCommandTimeScale);
	m_pOrderList->mUpdate(kDefaultScaleTime);

	return true;
}

/*
	�T�o�C�o�����[�h���̕`�揈��
*/
void SceneGame::mSurvivalRender(){
	if (m_nowMode != GameManager::eGameMode::eSurvaival)return;
	auto view = m_pPlayer->mGetView();
	view->Render();
	m_pPlayer->mRender(m_pixelShader.get(), m_pixelShader.get());

	m_penemyGround->mRender(m_pixelShader.get(),m_pixelShader.get());
	m_pFieldArea->mRender(m_pixelShader.get());

	
}

//
void SceneGame::mSurvivalUIRender(){
	if (m_nowMode != GameManager::eGameMode::eSurvaival)return;

	m_pActionBoard->mRender(m_pixelShader.get());
	m_pOrderList->mRender(m_pixelShader.get());
	
	
}


/*
	�v���N�e�B�X�ȉ���
*/
bool SceneGame::mPracticeMainUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::ePractice || m_gameState != eState::eRun)return true;

}

/*
 �v���Nry
*/
void SceneGame::mPracticeRender(){
	if (m_nowMode != GameManager::eGameMode::ePractice)return;
}

//
void SceneGame::mPracticeUIRender(){
	if (m_nowMode != GameManager::eGameMode::ePractice)return;
}

/*
	���U���g�̕\���p
*/
void SceneGame::mShowResult(GameManager::eDay nowDay, ShaderBase* defaultShader,ShaderBase* bularShader){
	if (m_gameState != eState::eResult)return;

	m_pResultBord->mSetupData(m_resultData, nowDay); // �����I�ɂ͈�񂵂��Ă΂�Ȃ�

	auto mouse = GameController::GetMouse().GetMousePosition();
	bool click = GameController::GetMouse().IsLeftButtonTrigger();
	auto state = m_pResultBord->mUpdate(mouse, click);

	if (state == ResultBord::eClickState::eExit){
		m_gameState = eState::eExit;
		return;
	}
	else if (state == ResultBord::eClickState::eNextDay){
		// ���̓��ɐi�ޏ���
		auto prevDay = m_nowDay;
		m_nowDay = m_dayHash[prevDay];
		m_gameState = eState::eFadeIn;
	}
	m_pResultBord->mRender(defaultShader, bularShader);
	return;
}

void SceneGame::mRegisterDayHash(GameManager::eDay key, GameManager::eDay value){
	if (m_dayHash.find(key) != m_dayHash.end())return;

	m_dayHash.insert(std::make_pair(key, value));

	return;
}

//
bool SceneGame::mFadeState(SceneGame::eState state){ 
	if (state == eState::eFadeIn || state == eState::eFadeOut){
		bool isEnd = false;
		switch (state)
		{
		case eState::eFadeIn:
			isEnd = m_pFadeObject->In(1.0f);
			if (isEnd){
				m_gameState = eState::eFadeOut;
			}
			break;

		case eState::eFadeOut:
			isEnd = m_pFadeObject->Out(1.0f);
			if (isEnd){
				m_gameState = eState::eRun;
			}
			break;
		}
		if (!isEnd) return false;
	}
	return true;
}

//
void SceneGame::mRegisterDay(){
	mRegisterDayHash(GameManager::eDay::e1day, GameManager::eDay::e2day);
	mRegisterDayHash(GameManager::eDay::e2day, GameManager::eDay::e3day);
	mRegisterDayHash(GameManager::eDay::e3day, GameManager::eDay::e4day);
	mRegisterDayHash(GameManager::eDay::e4day, GameManager::eDay::e5day);
	mRegisterDayHash(GameManager::eDay::e5day, GameManager::eDay::e6day);
	mRegisterDayHash(GameManager::eDay::e6day, GameManager::eDay::e7day);
	mRegisterDayHash(GameManager::eDay::e7day, GameManager::eDay::eLastDay);
	mRegisterDayHash(GameManager::eDay::eLastDay, GameManager::eDay::eNull);

	return;
}