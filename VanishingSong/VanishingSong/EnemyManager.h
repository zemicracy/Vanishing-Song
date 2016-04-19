#ifndef _ENEMYMANAGER_H
#define _ENEMYMANAGER_H
#include "EnemyAI.h"
#include "EnemyBase.h"
#include "EnemyGround.h"
#include"Player.h"


class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	bool mInitilize(aetherClass::ViewCamera*);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdater();
	void mFinalize();
	void mSetPosion();
	void mChangeAction();
	void mSpawn();

private:
	
	std::vector<std::shared_ptr<EnemyGround>> m_pEnemy;
	std::array<aetherClass::Vector3, 4>	m_pEnemySpawner;

};
#endif