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

	GameManager::mGetInstance().mBossState(GameManager::eBossState::eUnVisible);
//	GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eTutorial);

	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eBlue, "Model\\Player","Model\\Player\\blue");


	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eBlue, "Model\\Enemy\\Ground", "\\blue");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eRed, "Model\\Enemy\\Air", "\\red");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eGreen, "Model\\Enemy\\Air", "\\green");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eYellow, "Model\\Enemy\\Ground", "\\yellow");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eAdlib, "Model\\Enemy\\Ground", "\\yellow");

	return true;
}

//　１フレームごとに実行される
bool VanishingSongFrame::FrameRunningBuffer(){
	_heapmin();
	
	return true;
}

// プログラムの終了時に実行される
void VanishingSongFrame::FinalizeBuffer(){

	ResourceManager::mGetInstance().Finalize();
	return;
}