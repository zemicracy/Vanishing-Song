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
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eBlue, "Model\\Player","\\blue");
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eRed, "Model\\Player", "\\red");
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eGreen, "Model\\Player", "\\green");
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eYellow, "Model\\Player", "\\yellow");

	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eGreen, "Model\\Enemy\\Air");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eRed, "Model\\Enemy\\Ground");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eBlue, "Model\\Enemy\\Air");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eYellow, "Model\\Enemy\\Ground");

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