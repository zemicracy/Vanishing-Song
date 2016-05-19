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
	return true;
}

//�@�P�t���[�����ƂɎ��s�����
bool VanishingSongFrame::FrameRunningBuffer(){
	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(0);

	
	return true;
}

// �v���O�����̏I�����Ɏ��s�����
void VanishingSongFrame::FinalizeBuffer(){

	Singleton<ResourceManager>::GetInstance().Finalize();
	return;
}