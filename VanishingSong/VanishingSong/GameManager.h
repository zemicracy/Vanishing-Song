#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

class GameManager
{
public:

	enum class eGameMode{
		eBattle,
		eSurvaival,
		ePractice,
		eTutorial,
		eNull
	};
	enum class eState{
		e1PWin,
		e2PWin,
		eClear,
		eGameOver,
		eNull
	};

	enum class eSkillType{
		eExAttack,
		eExShield,
		eExHeel,
		eExBuff,
		eNull
	};
public:
	GameManager();
	~GameManager();
	void mInitialize();
	signed char mGetToDay();
	void mGameState(eState);
	eState mGameState();
	bool mIsPause();
	void mNextDay();
	void mCheckPauseKey();

	/*	スキルセット、取得用*/
	eSkillType mSkillType()const;
	void mSkillType(const eSkillType);

	/*	ゲームモードセット、取得用*/
	eGameMode mGameMode()const;
	void mGameMode(const eGameMode);
	
private:
	eState m_gameState;
	bool m_isPause;
	signed char m_day;
	eSkillType m_skillType;
	eGameMode m_gameMode;
	
};

#endif