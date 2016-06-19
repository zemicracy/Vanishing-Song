#pragma once
#include "GameManager.h"
class PlayDataManager
{
public:
	PlayDataManager();
	~PlayDataManager();
	
	void mSave();
	void mLoad();

public:
	static const std::string mFilePath;
private:
	GameManager::eFieldState mLoadFieldState(const int);
	int mSaveFieldState(GameManager::eFieldState)const;

	GameManager::eBossState mLoadBossState(const int);
	int mSaveBossState(GameManager::eBossState)const;

	char mGetTypeToChar(eMusical);
	eMusical mGetCharToType( char);

	
};

