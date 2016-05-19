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
	プログラムの開始時のみ実行される
*/
bool VanishingSongFrame::InitializeBuffer(){
	bool result = false;
	result = Singleton<ResourceManager>::GetInstance().Initialize();

	// 操作キャラクターのリソースを初期化
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eBlue, "Model\\Player","\\blue");
	Singleton<ResourceManager>::GetInstance().mPlayerInitialize(eMusical::eRed, "Model\\Player", "\\red");
	return true;
}

//　１フレームごとに実行される
bool VanishingSongFrame::FrameRunningBuffer(){
	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(0);

	
	return true;
}

// プログラムの終了時に実行される
void VanishingSongFrame::FinalizeBuffer(){

	Singleton<ResourceManager>::GetInstance().Finalize();
	return;
}