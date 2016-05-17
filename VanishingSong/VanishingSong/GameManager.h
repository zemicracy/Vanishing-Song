#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
struct BattlePlayerDesc{
	//std::array<>
};
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
		eNull,
	};

	enum class eGameMode{
		eNull,
		eQuarter,  // 四分
		eEighter,   // 八分
	};
public:
	GameManager();
	~GameManager();
	void mInitialize();
	bool mIsPause();	
private:
	bool m_isPause;
	
};

#endif