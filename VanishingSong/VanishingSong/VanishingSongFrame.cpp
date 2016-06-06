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
	result = Singleton<ResourceManager>::GetInstance().Initialize();
	// ����L�����N�^�[�̃��\�[�X��������
	Singleton<GameManager>::GetInstance().mPushUsePlayer(eMusical::eBlue);
	Singleton<GameManager>::GetInstance().mPushUsePlayer(eMusical::eGreen);
	Singleton<GameManager>::GetInstance().mPushUsePlayer(eMusical::eRed);
	Singleton<GameManager>::GetInstance().mPushUsePlayer(eMusical::eYellow);

	Singleton<GameManager>::GetInstance().mBossState(GameManager::eBossState::eUnVisible);
	Singleton<GameManager>::GetInstance().mFieldState(GameManager::eFieldState::eTutorial);

	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eBlue, "Model\\Player","\\blue");
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eRed, "Model\\Player", "\\red");
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eGreen, "Model\\Player", "\\green");
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eYellow, "Model\\Player", "\\yellow");



	return true;
}

//�@�P�t���[�����ƂɎ��s�����
bool VanishingSongFrame::FrameRunningBuffer(){

	
	return true;
}

// �v���O�����̏I�����Ɏ��s�����
void VanishingSongFrame::FinalizeBuffer(){

	Singleton<ResourceManager>::GetInstance().Finalize();
	return;
}