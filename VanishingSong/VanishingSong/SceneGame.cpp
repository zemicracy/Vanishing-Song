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
	const float kDefaultScaleTime = 1.0f;
}
const std::string SceneGame::Name = "Game";
SceneGame::SceneGame():
GameScene(Name, GetManager()) //Scene���Ƃ̖��O��ݒ�
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_dayTime = NULL;
}


SceneGame::~SceneGame()
{
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
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
	m_pPlayer = std::make_unique<Player>();
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

	m_pFieldArea = std::make_unique<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	// �Q�[�����[�h�̎擾
	// ���̃V�[���ɗ��Ă��鎞�_�ŁA�T�o�C�o�����A�v���N�e�B�X���I������Ă���
	m_nowMode = Singleton<GameManager>::GetInstance().mGameMode();
	m_nowDay = GameManager::eDay::e1day;
	mInitializeMode(m_nowMode);
	m_dayTime = 0.0f;
	
	RegisterScene(new SceneTitle());
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

	if (m_pFieldArea){
		m_pFieldArea.release();
		m_pFieldArea = nullptr;
	}

	if (m_pPlayer){
		m_pPlayer->mFinalize();
		m_pPlayer.release();
		m_pPlayer = nullptr;
	}

	if (m_penemyGround){
		m_penemyGround->mFinalize();
		m_penemyGround.reset();
		m_penemyGround = nullptr;
	}
	m_nowMode = GameManager::eGameMode::eNull;
	m_nowDay = GameManager::eDay::eNull;
	m_dayTime = NULL;
	return;
}

// �X�V����
bool SceneGame::Updater(){
	bool result = false;
	// �����̎擾
	m_dayTime += GameClock::GetDeltaTime();
	// �T�o�C�o�����[�h���̍X�V����
	result = this->mSurvivalUpdate(kDefaultScaleTime, m_dayTime);
	if (!result){
		return false;
	}

	// �Ճ��N�e�B���[�h���̍X�V����
	result = this->mPracticeUpdate(kDefaultScaleTime,m_dayTime);
	if (!result){
		return false;
	}
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
bool SceneGame::mSurvivalUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::eSurvaival)return true;

	m_pPlayer->mUpdate(kDefaultScaleTime);

	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	m_pPlayer->mCommand(m_pOrderList->mGetActionCommand(), kDefaultScaleTime);

	m_pActionBoard->mUpdate(kDefaultScaleTime);
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
bool SceneGame::mPracticeUpdate(const float timeScale, const float nowTime){
	if (m_nowMode != GameManager::eGameMode::ePractice)return true;

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
