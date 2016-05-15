#ifndef _FieldEnemyManager_H
#define _FieldEnemyManager_H
#include <map>
#include "FieldEnemy.h"
#include "GameClock.h"


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
	std::vector<std::shared_ptr<FieldEnemy>> mEnemyGet(int);

private:
	
	std::vector<std::shared_ptr<FieldEnemy>> m_pEnemy;
	std::array<std::vector<std::shared_ptr<FieldEnemy>>,4> m_enemyArray;
	std::array<aetherClass::Vector3, 4>	m_pEnemySpawner;	//oŒ»êŠ
	int EnemyGroundSize;	//Enemy’nã‚ÌÅ‘å”
	int EnemyAirSize;	//Enemy‹ó’†‚ÌÅ‘å”
	int EnemySize;	//Enemy‡Œv”
	
};
#endif