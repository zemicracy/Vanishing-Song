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

	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eBlue, eEnemyType::eAir, "Model\\Enemy\\Air", "\\Blue");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eRed, eEnemyType::eAir, "Model\\Enemy\\Air", "\\Red");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eGreen, eEnemyType::eAir, "Model\\Enemy\\Air", "\\Green");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eYellow, eEnemyType::eAir, "Model\\Enemy\\Air", "\\Yellow");


	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eBlue, eEnemyType::eGround, "Model\\Enemy\\Ground", "\\Blue");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eRed, eEnemyType::eGround, "Model\\Enemy\\Ground", "\\Red");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eGreen, eEnemyType::eGround, "Model\\Enemy\\Ground", "\\Green");
	Singleton<ResourceManager>::GetInstance().mEnemyInitialize(eMusical::eYellow, eEnemyType::eGround, "Model\\Enemy\\Ground", "\\Yellow");

	return true;
}

//　１フレームごとに実行される
bool VanishingSongFrame::FrameRunningBuffer(){

	
	return true;
}

// プログラムの終了時に実行される
void VanishingSongFrame::FinalizeBuffer(){

	Singleton<ResourceManager>::GetInstance().Finalize();
	return;
}