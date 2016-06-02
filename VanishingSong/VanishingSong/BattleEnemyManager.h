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
	void Initialize(aetherClass::ViewCamera* camera,BattleField* );

	void mUpadate(const float timeScale);

	std::vector<eMusical> GetList();

	void AddList();

	/*
		描画処理
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);

	/*
		敵が全滅時にtrueを返すようにする
	*/
	bool mIsEnd();

private:
	void mLoadInfo(std::string);
private:
	
	void BlueAdd();
	void RedAdd();
	void GreenAdd();

	bool m_isEnd;
	std::vector<std::shared_ptr<BattleEnemy>> m_pEnemy;
	BattleField* m_BattleField;
	std::vector<eMusical> m_enemyList;

};

#endif