#include "PlayDataManager.h"
#include "ResourceManager.h"
#include <fstream>
#include "Cipher.h"
const std::string PlayDataManager::mSaveFile = "data\\PlayData\\save_data";
const std::string PlayDataManager::mConfigFile = "data\\PlayData\\config";
PlayDataManager::PlayDataManager()
{
}


PlayDataManager::~PlayDataManager()
{
}

void PlayDataManager::mSave(){
	auto fieldState = GameManager::mGetInstance().mFieldState();
	std::ofstream saveObject;
	saveObject.open(mSaveFile, std::ios::out);
	saveObject << "[Field]" << std::endl;
	saveObject << mSaveFieldState(fieldState) << std::endl;

	const int canStage = GameManager::mGetInstance().mGetCanStage();
	saveObject << "[Stage]" << std::endl;
	saveObject << canStage << std::endl;

	auto bossState = GameManager::mGetInstance().mBossState();
	saveObject << "[Boss]" << std::endl;
	saveObject << mSaveBossState(bossState) << std::endl;

	auto& usePlayer = GameManager::mGetInstance().mGetUsePlayer();
	saveObject << "[UsePlayer]" << std::endl;
	for (auto& use : usePlayer){
		saveObject << mGetTypeToChar(use.second) << ',';
	}
	saveObject << std::endl;

	saveObject << "[BGM]" << std::endl;
	auto& getBGM = ResourceManager::mGetInstance().mGetBGMPath();
	for (auto& bgm : getBGM){
		saveObject << mGetTypeToChar(bgm.first) << ',' << bgm.second << std::endl;
	}

	// ˆÃ†‰»
	Cipher lock;
	lock.mLock(mSaveFile);
	return;
}

void PlayDataManager::mLoad(){
	Cipher load;
	load.mLoadFile(mSaveFile);
	auto fieldState = mLoadFieldState(std::atoi(load.mGetData("[Field]", 0).c_str()));
	GameManager::mGetInstance().mFieldState(fieldState);

	auto bossState = mLoadBossState(std::atoi(load.mGetData("[Boss]", 0).c_str()));
	GameManager::mGetInstance().mBossState(bossState);

	auto canStage = std::atoi(load.mGetData("[Stage]", 0).c_str());
	GameManager::mGetInstance().mGetCanStage(canStage);

	for (auto& splite : load.mGetSpriteArray("[UsePlayer]")){
		auto type = mGetCharToType(splite.front());
		GameManager::mGetInstance().mPushUsePlayer(type);
	}

	eMusical key;
	for (auto& splite : load.mGetSpriteArray("[BGM]")){
		eMusical type = mGetCharToType(splite.front());
		if (type == eMusical::eNull){
			ResourceManager::mGetInstance().mSetBGMPath(key) = splite;
		}
		else{
			key = type;
		}
	}
	return;
}


GameManager::eFieldState PlayDataManager::mLoadFieldState(const int id){
	switch (id)
	{
	case 0:
		return GameManager::eFieldState::eTutorial;

	case 1:
		return GameManager::eFieldState::eFirstStage;

	case 2:
		return GameManager::eFieldState::eSecoundStage;
	case 3:
		return GameManager::eFieldState::eThirdStage;
	case 4:
		return GameManager::eFieldState::eForthStage;
	case 5:
		return GameManager::eFieldState::eBoss;

	default:
		return GameManager::eFieldState::eNull;
	}
}


int PlayDataManager::mSaveFieldState(GameManager::eFieldState state)const{
	switch (state)
	{
	case GameManager::eFieldState::eTutorial:
		return 0;
	case GameManager::eFieldState::eFirstStage:
		return 1;
	case GameManager::eFieldState::eSecoundStage:
		return 2;
	case GameManager::eFieldState::eThirdStage:
		return 3;
	case GameManager::eFieldState::eForthStage:
		return 4;
	case GameManager::eFieldState::eBoss:
		return 5;
	case GameManager::eFieldState::eNull:
	default:
		return -1;
	}
}

GameManager::eBossState PlayDataManager::mLoadBossState(const int id){
	switch (id)
	{

	case 0:
		return GameManager::eBossState::eUnVisible;
	case 1:
		return GameManager::eBossState::eVisible;
	case 2:
		return GameManager::eBossState::eWin;
	default:
		return GameManager::eBossState::eNull;
	}
}


int PlayDataManager::mSaveBossState(GameManager::eBossState state)const{
	switch (state)
	{
	case GameManager::eBossState::eUnVisible:
		return 0;
	case GameManager::eBossState::eVisible:
		return 1;
	case GameManager::eBossState::eWin:
		return 2;
	case GameManager::eBossState::eNull:
	default:
		return -1;
	}
}


char PlayDataManager::mGetTypeToChar(eMusical type){
	switch (type)
	{
	case eMusical::eRed:
		return 'r';
	case eMusical::eBlue:
		return 'b';
	case eMusical::eYellow:
		return 'y';
	case eMusical::eGreen:
		return 'g';
	default:
		break;
	}
}

//
eMusical PlayDataManager::mGetCharToType(const char type){

	if (type == 'r'){
		return eMusical::eRed;
	}

	if (type == 'b'){
		return eMusical::eBlue;
	}

	if (type == 'y'){
		return eMusical::eYellow;
	}

	if (type == 'g'){
		return eMusical::eGreen;
	}

	return eMusical::eNull;
}

//
void PlayDataManager::mConfigSave(){
	auto fieldState = GameManager::mGetInstance().mFieldState();
	std::ofstream saveObject;
	saveObject.open(mConfigFile, std::ios::out);
	saveObject << "[Volume]" << std::endl;
	saveObject << GameManager::mGetInstance().mGetVolume() << std::endl;

	// ˆÃ†‰»
	Cipher lock;
	lock.mLock(mConfigFile);
}

//
void PlayDataManager::mConfigLoad(){
	Cipher load;
	load.mLoadFile(mConfigFile);
	auto volume = std::atoi(load.mGetData("[Volume]", 0).c_str());
	GameManager::mGetInstance().mSetVolume(volume);

	return;
}