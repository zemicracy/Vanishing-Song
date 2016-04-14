#include "VanishingSongFrame.h"
#include "Debug.h"
#include <Singleton.h>
#include "ResourceManager.h"
#include "GameManager.h"
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
	return true;
}

//�@�P�t���[�����ƂɎ��s�����
bool VanishingSongFrame::FrameRunningBuffer(){
	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(0);
	// �|�[�Y�{�^���������ꂽ���`�F�b�N����
	Singleton<GameManager>::GetInstance().CheckPauseKey();

	return true;
}

// �v���O�����̏I�����Ɏ��s�����
void VanishingSongFrame::FinalizeBuffer(){

	Singleton<ResourceManager>::GetInstance().Finalize();
	return;
}