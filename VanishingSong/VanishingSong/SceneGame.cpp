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
	const float kScaleTime = 1.0f; 
	const bool kError = false;
	const float kDayTime = 180.0f; // ����̎���
}

SceneGame::SceneGame():
GameScene(Name, GetManager()) //Scene���Ƃ̖��O��ݒ�
{

	m_day = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{

	m_day = GameManager::eDay::eNull;
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}

bool SceneGame::Initialize(){
	bool result = true;

	Finalize();

	// �V�[���̓o�^
	RegisterScene(new SceneTitle());

	// �t�F�[�h�C���E�A�E�g���s��
	m_pFadeObject = std::make_unique<FadeManager>();

	// �Q�[�����[�h�̎擾
	// ���̃V�[���ɗ��Ă��鎞�_�ŁA�T�o�C�o�����A�v���N�e�B�X���I������Ă���
	auto mode = Singleton<GameManager>::GetInstance().mGameMode(); 
	mode = GameManager::eGameMode::eSurvaival;

	
	m_pResultBord = std::make_unique<ResultBord>();
	m_pResultBord->mInitialize(mode);
	
	// ���݂̃��[�h�̎擾
	m_pMode = mReturnMode(mode);

	// �X�L���̎擾
	auto skill = Singleton<GameManager>::GetInstance().mSkillType();
	skill = GameManager::eSkillType::eExAttack;

	result = m_pMode->mInitialize(skill,m_day);
	if (!result){
		Debug::mErrorPrint("���[�h�̏��������s", __FILE__, Debug::eState::eWindow);
		return false;
	}
	// �Q�[���̏�Ԃ�o�^
	m_gameState = eState::eRun;

	return true;
}

// �������
// �S�Ẳ��
void SceneGame::Finalize(){
	
	if (m_pMode){
		m_pMode->mFinalize();
		m_pMode.reset();
		m_pMode = nullptr;
	}
	
	if (m_pResultBord){
		m_pResultBord->mFinalize();
		m_pResultBord.release();
		m_pResultBord = nullptr;
	}

	m_day = GameManager::eDay::eNull;
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

	if (GameController::GetKey().KeyDownTrigger(VK_ESCAPE)){
		m_gameState = eState::eExit;
	}

	// �f�o�b�O�p
	if (GameController::GetKey().KeyDownTrigger('T')){
		m_gameState = eState::eResult;
	}

	if (m_gameState == eState::eExit){
		// �I���Ȃ�^�C�g���ɖ߂�
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return true;
	}

	if (m_gameState == eState::eResult)return true;

	// �����̎擾
	m_dayTime += (float)GameClock::GetDeltaTime();
	
	// �Ō�̓��ȊO�͐������ԂɂȂ����烊�U���g��ʂɍs��
	if (m_dayTime > kDayTime && m_day != GameManager::eDay::eLastDay){
		m_gameState = eState::eResult;
	}

	bool result = false;

	// ���ꂼ��̃��[�h�̍X�V����
	m_pMode->mMainUpdate(kScaleTime, m_dayTime);
	
	// ��Ԃ̍X�V
	auto state = m_pMode->mGetState();

	// ���ɂȂ��Ȃ牽�����Ȃ�
	if (state == Mode::eState::eNull)return true;

	switch (state)
	{
	case Mode::eState::eClear:
		m_gameState = eState::eResult;
		break;
	case Mode::eState::eGameOver:
	//	m_gameState = eState::eGameOver;
		break;
	case Mode::eState::eNextDay:
		m_gameState = eState::eResult;
		break;

	default:
		break;
	}
	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pMode->mMainRender(shaderHash);
	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

	m_pMode->mMainUIRender(shaderHash);

	mShowResult(m_day, shaderHash["texture"].get(), shaderHash["color"].get());

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
		m_day = GameManager::eDay::e1day;
		mRegisterDay();
		Debug::mPrint("�T�o�C�o�����[�h");

		return std::make_shared<ModeSurvival>();

	case GameManager::eGameMode::ePractice:
		Debug::mPrint("�v���N�e�B�X���[�h");
		return std::make_shared<ModePractice>();

	case GameManager::eGameMode::eNull:
	default:
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

	m_pResultBord->mSetupData(m_pMode->mGetResultData(), nowDay); // �����I�ɂ͈�񂵂��Ă΂�Ȃ�

	auto mouse = GameController::GetMouse().GetMousePosition();
	bool click = GameController::GetMouse().IsLeftButtonTrigger();
	auto state = m_pResultBord->mUpdate(mouse, click);

	if (state == ResultBord::eClickState::eExit){
		m_gameState = eState::eExit;
		return;
	}
	else if (state == ResultBord::eClickState::eNextDay){
		// ���̓��ɐi�ޏ���
		auto prevDay = m_day;
		m_pMode->mPrevDayFinalize(prevDay);
		m_day = m_dayHash[prevDay];
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
			m_pMode->mNextDayInitialize(m_day);
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