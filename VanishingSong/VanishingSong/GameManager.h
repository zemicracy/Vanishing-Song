#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
#include "Const.h"
#include <unordered_map>
#include <Transform.h>
class GameManager
{
public:
	// 現在の状態
	enum class eBattleState{
		eWin,
		eLose,
		eListen,    // 敵の演奏を聴く状態
		ePerform,   // プレイヤーの演奏開始状態 
		eBattle,    // 戦闘の実行状態
		eCheck,     // 今の状態を判定する
		eResult,	// リザルト
		eNull,
	};

	enum class eGameMode{
		eNull,
		eQuarter,  // 四分
		eEighter,   // 八分
	};

	// フィールドでのボスの状態
	enum class eBossState{
		eVisible,
		eUnVisible,
		eWin,
		eNull
	};

	enum class eFieldState{
		eTutorial,
		
		eFirstStage,
		eSecoundStage,
		eThirdStage,
		eForthStage,
		
		eBoss,
		eNull
	};
public:
	GameManager();
	~GameManager();
	void mInitialize();	

	void mPushUsePlayer(eMusical);
	std::unordered_map<eMusical, eMusical>& mGetUsePlayer();
	aetherClass::Transform mGetPlayerTransform();
	void mSetPlayerTransform(aetherClass::Transform);

	void mBattleDataFile(std::string);
	std::string mBattleDataFile();

	void mBossState(eBossState);
	eBossState mBossState();

	void mFieldState(eFieldState);
	eFieldState mFieldState();

	void mNote(eMusical);
	std::vector<eMusical>& mNote();
private:
	bool m_isPause;
	std::unordered_map<eMusical,eMusical> m_players;
	aetherClass::Transform m_prevPlayerTransform;
	std::string m_battleDataFile;
	eBossState m_bossState;
	eFieldState m_fieldState;
	std::vector<eMusical> m_noteArray;
};

#endif