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
	result = ResourceManager::mGetInstance().Initialize();
	// 操作キャラクターのリソースを初期化
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

//　１フレームごとに実行される
bool VanishingSongFrame::FrameRunningBuffer(){

	
	return true;
}

// プログラムの終了時に実行される
void VanishingSongFrame::FinalizeBuffer(){

	ResourceManager::mGetInstance().Finalize();
	return;
}