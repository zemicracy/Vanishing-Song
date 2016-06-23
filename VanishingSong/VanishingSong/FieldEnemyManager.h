#ifndef _FieldEnemyManager_H
#define _FieldEnemyManager_H
#include <map>
#include "FieldEnemy.h"
#include "GameClock.h"
#include "MessageWindow.h"
#include "GameManager.h"

class FieldEnemyManager
{

public:
	FieldEnemyManager();
	~FieldEnemyManager();

	bool mInitilize(aetherClass::ViewCamera*);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdater();
	void mFinalize();
	void mSetPosion();
	bool mGetIsJudge();
	std::shared_ptr<FieldEnemy>& mEnemyGet(int);
	bool mGetBossFlg();
	void mResetEnemysTransform();
private:

	int mFieldStateToInt(GameManager::eFieldState);

	std::vector<std::shared_ptr<FieldEnemy>> m_pEnemyList;
	std::array<aetherClass::Vector3, 5>	m_pEnemySpawner;	//èoåªèÍèä
	
	bool m_bossFlag;
	int EnemySize;	//Enemyêî
	int EnemyMaxSize;
	bool m_isRender;
	bool m_isJudge;
};
#endif