#pragma once
#include "GameManager.h"
class PlayDataManager
{
public:
	PlayDataManager();
	~PlayDataManager();
	
	void mSave();
	void mLoad();
	void mConfigSave();
	void mConfigLoad();

public:
	static const std::string mSaveFile;
	static const std::string mConfigFile;
private:
	GameManager::eFieldState mLoadFieldState(const int);
	int mSaveFieldState(GameManager::eFieldState)const;

	GameManager::eBossState mLoadBossState(const int);
	int mSaveBossState(GameManager::eBossState)const;

	char mGetTypeToChar(eMusical);
	eMusical mGetCharToType( char);

	
};

