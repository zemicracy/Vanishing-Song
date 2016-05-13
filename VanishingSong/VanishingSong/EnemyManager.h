#ifndef _ENEMYMANAGER_H
#define _ENEMYMANAGER_H
#include "EnemyAI.h"
#include <map>
#include "EnemyBase.h"
#include "EnemyGround.h"
#include "GameClock.h"



class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	struct AreaProperty{
		int area_max_x;
		int area_min_x;
		int area_max_z;
		int area_min_z;
	};
	
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
	std::vector<std::shared_ptr<EnemyGround>> mEnemyGet(int);

private:
	
	std::vector<std::shared_ptr<EnemyGround>> m_pEnemy;
	std::array<std::vector<std::shared_ptr<EnemyGround>>,4> m_enemyArray;
	std::array<aetherClass::Vector3, 4>	m_pEnemySpawner;
	AreaProperty areaProperty[4];
	float time;
	float actiontime;
	int m_Enemy_Max;
	
};
#endif