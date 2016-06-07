#include "VanishingSongFrame.h"
#include "Debug.h"
#include <Singleton.h>
#include "ResourceManager.h"
#include "GameManager.h"
#include "GameController.h"
#include "Const.h"
using namespace aetherClass;
VanishingSongFrame::VanishingSongFrame()
{
}


VanishingSongFrame::~VanishingSongFrame()
{
}


/*
	�v���O�����̊J�n���̂ݎ��s�����
*/
bool VanishingSongFrame::InitializeBuffer(){
	bool result = false;
	result = ResourceManager::mGetInstance().Initialize();
	// ����L�����N�^�[�̃��\�[�X��������
	GameManager::mGetInstance().mPushUsePlayer(eMusical::eBlue);
	GameManager::mGetInstance().mPushUsePlayer(eMusical::eGreen);
	GameManager::mGetInstance().mPushUsePlayer(eMusical::eRed);
	GameManager::mGetInstance().mPushUsePlayer(eMusical::eYellow);

	GameManager::mGetInstance().mBossState(GameManager::eBossState::eUnVisible);
	GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eTutorial);

	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eBlue, "Model\\Player","\\blue");
	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eRed, "Model\\Player", "\\red");
	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eGreen, "Model\\Player", "\\green");
	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eYellow, "Model\\Player", "\\yellow");



	return true;
}

//�@�P�t���[�����ƂɎ��s�����
bool VanishingSongFrame::FrameRunningBuffer(){

	
	return true;
}

// �v���O�����̏I�����Ɏ��s�����
void VanishingSongFrame::FinalizeBuffer(){

	ResourceManager::mGetInstance().Finalize();
	return;
}