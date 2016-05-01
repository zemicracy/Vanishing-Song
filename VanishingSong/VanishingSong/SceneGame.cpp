#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
#include "ResourceManager.h"
#include "ModeSurvival.h"
#include "ModePractice.h"
using namespace aetherClass;

const std::string SceneGame::Name = "Game";

namespace{
	const float kDefaultScaleTime = 100.0f; 
	const float kCommandTimeScale = 1.0f;
	const bool kError = false;
	const float kDayTime = 300.0f;
}

SceneGame::SceneGame():
GameScene(Name, GetManager()) //Scene���Ƃ̖��O��ݒ�
{

	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{

	m_nowDay = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}

bool SceneGame::Initialize(){

	Finalize();

	// �V�[���̓o�^
	RegisterScene(new SceneTitle());

	bool result = false;

	// �A�N�V�����R�}���h�̏�����
	m_pActionBoard = std::make_unique<ActionBoard>();
	auto skill = Singleton<GameManager>::GetInstance().mSkillType();
	skill = GameManager::eSkillType::eExAttack;
	m_pActionBoard->mInitialize(skill);

	// �I�[�_�[���X�g�̏�����
	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize();

	// �t�F�[�h�C���E�A�E�g���s��
	m_pFadeObject = std::make_unique<FadeManager>();

	// �Q�[�����[�h�̎擾
	// ���̃V�[���ɗ��Ă��鎞�_�ŁA�T�o�C�o�����A�v���N�e�B�X���I������Ă���
	auto nowMode = Singleton<GameManager>::GetInstance().mGameMode(); 
	nowMode = GameManager::eGameMode::eSurvaival;

	
	m_pResultBord = std::make_unique<ResultBord>();
	m_pResultBord->mInitialize(nowMode);
	
	// ���݂̃��[�h�̎擾
	m_pMode = mReturnMode(nowMode);
	m_pMode->mInitialize(m_nowDay);

	// �Q�[���̏�Ԃ�o�^
	m_gameState = eState::eRun;

	return true;
}

// �������
// �S�Ẳ��
void SceneGame::Finalize(){
	if (m_pActionBoard){
		m_pActionBoard.release();
		m_pActionBoard = nullptr;
	}

	if (m_pOrderList){
		m_pOrderList.release();
		m_pOrderList = nullptr;
	}

	if (m_pResultBord){
		m_pResultBord->mFinalize();
		m_pResultBord.release();
		m_pResultBord = nullptr;
	}

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
		return true;
	}

	bool result = false;
	// �����̎擾
	m_dayTime += GameClock::GetDeltaTime();

	if (m_dayTime > kDayTime){
		m_gameState = eState::eResult;
	}

	//m_pActionBoard->mUpdate(kCommandTimeScale);
	
	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	// ���ꂼ��̃��[�h�̍X�V����
	m_pMode->mMainUpdate(m_pOrderList->mGetActionCommand(), kDefaultScaleTime, m_dayTime);
	
	m_pActionBoard->mUpdate(kCommandTimeScale);
	m_pOrderList->mUpdate(kDefaultScaleTime);

	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pMode->mMainRender(shaderHash);
	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	m_pActionBoard->mRender(shaderHash["color"].get());

	m_pOrderList->mRender(shaderHash["color"].get());

	mShowResult(m_nowDay, shaderHash["color"].get(), shaderHash["color"].get());

	if (m_gameState == eState::eFadeIn || m_gameState == eState::eFadeOut){
		m_pFadeObject->mRedner(shaderHash["color"].get());
	}
	return;
}

bool SceneGame::TransitionIn(){

	return kTransitionEnd;
}

bool SceneGame::TransitionOut(){

	return kTransitionEnd;
}


std::shared_ptr<Mode> SceneGame::mReturnMode(GameManager::eGameMode mode){
	switch (mode)
	{
	case GameManager::eGameMode::eSurvaival:
		m_nowDay = GameManager::eDay::e1day;
		mRegisterDay();
		Debug::mPrint("�T�o�C�o�����[�h");

		return std::make_shared<ModeSurvival>();

	case GameManager::eGameMode::ePractice:
		Debug::mPrint("�v���N�e�B�X���[�h");
		return std::make_shared<ModePractice>();

	case GameManager::eGameMode::eNull:
		assert(!"�s���Ȓl�����o���܂��� :Part SG");
		return nullptr;
	}
	return nullptr;
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
		m_pMode->mPrevDayFinalize(prevDay);
		m_nowDay = m_dayHash[prevDay];
		m_gameState = eState::eFadeIn;
		m_dayTime = NULL;
	}
	m_pResultBord->mRender(defaultShader, bularShader);

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
			// ���̓��ɐi�ނɂ������Ă̏���������
			m_pMode->mNextDayInitialize(m_nowDay);
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

void SceneGame::mRegisterDayHash(GameManager::eDay key, GameManager::eDay value){
	if (m_dayHash.find(key) != m_dayHash.end())return;

	m_dayHash.insert(std::make_pair(key, value));

	return;
}