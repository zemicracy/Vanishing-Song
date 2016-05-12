#include "SceneGame.h"
#include <iostream>
#include <PixelShader.h>
#include <GameController.h>
#include <GameClock.h>
#include "GameManager.h"
#include <Singleton.h>
#include "SceneTitle.h"
#include "ResourceManager.h"
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
	m_gameState = eState::eNull;
	m_dayTime = NULL;

}


SceneGame::~SceneGame()
{

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
	
	// �X�L���̎擾
	auto skill = Singleton<GameManager>::GetInstance().mSkillType();
	skill = GameManager::eSkillType::eExHeel;

	// �Q�[���̏�Ԃ�o�^
	m_gameState = eState::eRun;

	std::cout << "�I��" << std::endl;
	return true;
}

// �������
// �S�Ẳ��
void SceneGame::Finalize(){

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


	if (m_gameState == eState::eExit){
		// �I���Ȃ�^�C�g���ɖ߂�
		ChangeScene(SceneTitle::Name, LoadState::eUse);
		return true;
	}

	// �����̎擾
	m_dayTime += (float)GameClock::GetDeltaTime();
	return true;
}

void SceneGame::Render(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	return;
}

void SceneGame::UIRender(){
	auto shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();

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