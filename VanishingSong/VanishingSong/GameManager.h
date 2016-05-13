#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
class GameManager
{
public:
	// 現在の状態
	enum class eGameState{
		e1PWin,
		eClear,
		eGameOver,
		eListen,    // 敵の演奏を聴く状態
		ePerform,   // プレイヤーの演奏開始状態 
		eBattle,    // 戦闘の実行状態
		eNull,
	};
public:
	GameManager();
	~GameManager();
	void mInitialize();
	void mGameState(eGameState);
	eGameState mGameState();
	bool mIsPause();	
private:
	eGameState m_gameState;
	bool m_isPause;
	
};

#endif