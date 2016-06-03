#ifndef _BATTLEENEMYMANAGER_H
#define _BATTLEENEMYMANAGER_H
#include "BattleEnemy.h"
#include"BattleField.h"
#include <ShaderBase.h>
#include <array>
namespace{
	const int kEnemyMaxNum = 4;
}
class BattleEnemyManager
{
	

public:
	BattleEnemyManager();
	~BattleEnemyManager();
	/*
		初期化時の引数でどの敵を配置するかを取得する
		_typeがNullの場合は初期化の処理を飛ばす
	*/
	void mInitialize(aetherClass::ViewCamera* camera,BattleField* );

	void mUpadate(const float timeScale);

	std::vector<eMusical> GetList();

	void ResetEnemyList(int,aetherClass::ViewCamera*camera);

	void misDie();

	/*
		描画処理
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);

	/*
		敵が全滅時にtrueを返すようにする
	*/
	bool mIsEnd();

	int mGetWaveAllCount();

private:
	void mLoadInfo(std::string, BattleField*, aetherClass::ViewCamera*);
	int mGetRandom();
	eMusical mGetEnemyColor(const char);
	eEnemyType mGetEnemyType(const char);
	eMusical mGetEnemyAttack(const char);

private:
	
	BattleField* m_BattleField;
	std::vector<std::shared_ptr<BattleEnemy>> m_pEnemy;
	std::vector<eMusical> m_enemyList;
	std::vector<std::vector<std::pair<eMusical, eEnemyType>>> m_waveEnemyList;
	std::vector<std::vector<eMusical>> m_enemyAttackList;

	bool m_isEnd;
	int m_waveAllCount;
	int m_attackAllCount;
};

#endif