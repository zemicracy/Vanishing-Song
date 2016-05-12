#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
class GameManager
{
public:
	// 選択されたモード用
	enum class eGameMode{
		eBattle,
		eGame,
		ePractice,
		eTutorial,
		eNull
	};

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

	// スキルのタイプ
	enum class eSkillType{
		eExAttack,
		eExShield,
		eExHeel,
		eExBuff,
		eNull
	};

	// 日数
	enum class eDay{
		eNull,
		e1day,
		e2day,
		e3day,
		e4day,
		e5day,
		e6day,
		e7day,
		eLastDay,
	};
public:
	GameManager();
	~GameManager();
	void mInitialize();
	signed char mGetToDay();
	void mGameState(eGameState);
	eGameState mGameState();
	bool mIsPause();
	void mNextDay();

	/*	スキルセット、取得用*/
	eSkillType mSkillType()const;
	void mSkillType(const eSkillType);

	/*	ゲームモードセット、取得用*/
	eGameMode mGameMode()const;
	void mGameMode(const eGameMode);
	
private:
	eGameState m_gameState;
	bool m_isPause;
	signed char m_day;
	eSkillType m_skillType;
	eGameMode m_gameMode;
	
};

#endif