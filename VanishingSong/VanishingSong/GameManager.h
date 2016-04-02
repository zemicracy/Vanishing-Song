#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

class GameManager
{
public:
	enum class eState{
		e1PWin,
		e2PWin,
		eClear,
		eGameOver,
		eNull
	};
public:
	GameManager();
	~GameManager();
	void Initialize();
	signed char GetToDay();
	void GameState(eState);
	eState GameState();
	bool IsPause();
	void NextDay();
	void CheckPauseKey();
private:
	eState m_gameState;
	bool m_isPause;
	signed char m_day;
};

#endif