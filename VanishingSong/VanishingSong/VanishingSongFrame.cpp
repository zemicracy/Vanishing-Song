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
	プログラムの開始時のみ実行される
*/
bool VanishingSongFrame::InitializeBuffer(){
	bool result = false;
	result = Singleton<ResourceManager>::GetInstance().Initialize();
	return true;
}

//　１フレームごとに実行される
bool VanishingSongFrame::FrameRunningBuffer(){
	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(0);
	// ポーズボタンが押されたかチェックする
	Singleton<GameManager>::GetInstance().mCheckPauseKey();

	return true;
}

// プログラムの終了時に実行される
void VanishingSongFrame::FinalizeBuffer(){

	Singleton<ResourceManager>::GetInstance().Finalize();
	return;
}