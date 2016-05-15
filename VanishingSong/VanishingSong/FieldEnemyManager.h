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
	std::array<aetherClass::Vector3, 4>	m_pEnemySpawner;	//�o���ꏊ
	int EnemyGroundSize;	//Enemy�n��̍ő吔
	int EnemyAirSize;	//Enemy�󒆂̍ő吔
	int EnemySize;	//Enemy���v��
	
};
#endif