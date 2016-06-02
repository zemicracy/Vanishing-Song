#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
#include "Const.h"
#include <unordered_map>
#include <Transform.h>
class GameManager
{
public:
	// Œ»İ‚Ìó‘Ô
	enum class eBattleState{
		eWin,
		eLose,
		eListen,    // “G‚Ì‰‰‘t‚ğ’®‚­ó‘Ô
		ePerform,   // ƒvƒŒƒCƒ„[‚Ì‰‰‘tŠJnó‘Ô 
		eBattle,    // í“¬‚ÌÀsó‘Ô
		eCheck,     // ¡‚Ìó‘Ô‚ğ”»’è‚·‚é
		eResult,	// ƒŠƒUƒ‹ƒg
		eNull,
	};

	enum class eGameMode{
		eNull,
		eQuarter,  // l•ª
		eEighter,   // ”ª•ª
	};

	// ãƒ•ã‚£ãƒ¼ãƒ«ãƒ‰ã§ã®ãƒœã‚¹ã®çŠ¶æ…‹
	enum class eBossState{
		eVisible,
		eUnVisible,
		eWin,
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

	void mFieldBossState(eBossState);
	eBossState mFieldBossState();
private:
	bool m_isPause;
	std::unordered_map<eMusical,eMusical> m_players;
	aetherClass::Transform m_prevPlayerTransform;
	std::string m_battleDataFile;
	eBossState m_fieldBossState;
};

#endif