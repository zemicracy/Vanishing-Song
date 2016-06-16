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

	GameManager::mGetInstance().mBossState(GameManager::eBossState::eUnVisible);
	GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eTutorial);

	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eBlue, "Model\\Player","Model\\Player\\blue");

	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eBlue, "Model\\Enemy\\Air\\Air.fbx", "Model\\Enemy\\Air\\tex");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eYellow, "Model\\Enemy\\annon\\annon.fbx", "Model\\Enemy\\annon\\tex");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eGreen, "Model\\Enemy\\Ground\\Gro.fbx", "Model\\Enemy\\Ground\\tex");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eRed, "Model\\Enemy\\Ground\\body.fbx", "Model\\Enemy\\Ground\\yellow");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eAdlib, "Model\\Enemy\\Boss\\model.fbx", "Model\\Enemy\\Boss\\tex");

	return true;
}

//�@�P�t���[�����ƂɎ��s�����
bool VanishingSongFrame::FrameRunningBuffer(){
	_heapmin();
	
	return true;
}

// �v���O�����̏I�����Ɏ��s�����
void VanishingSongFrame::FinalizeBuffer(){

	ResourceManager::mGetInstance().Finalize();
	return;
}