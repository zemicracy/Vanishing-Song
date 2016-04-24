#ifndef _ENEMYMANAGER_H
#define _ENEMYMANAGER_H
#include "EnemyAI.h"
#include <map>
#include "EnemyBase.h"
#include "EnemyGround.h"
#include "GameClock.h"
#include"Player.h"


class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();


	
	bool mInitilize(aetherClass::ViewCamera*);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mStatusInit();
	void mStatusUpdater();
	void mUpdater();
	void mFinalize();
	void mSetPosion();
	void mSetMap();
	void mChangeAction();
	void mSpawn();

private:
	
	std::vector<std::shared_ptr<EnemyGround>> m_pEnemy;
	std::map<std::string, std::shared_ptr<EnemyGround>> m_EnemyMap;
	std::array<aetherClass::Vector3, 4>	m_pEnemySpawner;
	float time;
	int m_Enemy_Max;

};
#endif