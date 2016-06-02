#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
#include "Const.h"
#include <unordered_map>
#include <Transform.h>
class GameManager
{
public:
	// ťÝĚóÔ
	enum class eBattleState{
		eWin,
		eLose,
		eListen,    // GĚtđŽ­óÔ
		ePerform,   // vC[ĚtJnóÔ 
		eBattle,    // íŹĚŔsóÔ
		eCheck,     // ĄĚóÔđťčˇé
		eResult,	// Ug
		eNull,
	};

	enum class eGameMode{
		eNull,
		eQuarter,  // lŞ
		eEighter,   // ŞŞ
	};

	// ăăŁăźăŤăă§ăŽăăšăŽçść
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