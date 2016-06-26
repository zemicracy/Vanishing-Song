#include "VanishingSongFrame.h"
#include "Debug.h"
#include <Singleton.h>
#include "ResourceManager.h"
#include "GameManager.h"
#include "GameController.h"
#include "Const.h"
#include "PlayDataManager.h"
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

	GameManager::mGetInstance().mBossState(GameManager::eBossState::eUnVisible);
	GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eTutorial);

	ResourceManager::mGetInstance().mPlayerInitialize(eMusical::eBlue, "Model\\Player\\blue.fbx", "Model\\Player\\blue");
	
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eBlue, "Model\\Enemy\\Air\\air.fbx", "Model\\Enemy\\Air\\tex");
	ResourceManager::mGetInstance().mEnemyInitialize(eMusical::eYellow, "Model\\Enemy\\Annon\\annon.fbx", "Model\\Enemy\\annon\\tex");
	
	// 初めに設定ファイルだけはロードしておく
	PlayDataManager playData;
	playData.mConfigLoad();

	return true;
}

//　１フレームごとに実行される		ResourceManager::mGetInstance が返されました	{m_pTextureHash=??? m_pShaderHash=??? m_pActionSoundHash=??? ...}	ResourceManager &
bool VanishingSongFrame::FrameRunningBuffer(){
	_heapmin();
	
	return true;
}

// プログラムの終了時に実行される
void VanishingSongFrame::FinalizeBuffer(){
	PlayDataManager playData;
	playData.mConfigSave();
	ResourceManager::mGetInstance().Finalize();
	return;
}