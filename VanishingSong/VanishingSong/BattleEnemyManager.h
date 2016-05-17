#ifndef _BATTLEENEMYMANAGER_H
#define _BATTLEENEMYMANAGER_H
#include "BattleEnemy.h"
#include <ShaderBase.h>
#include <array>
namespace{
	const int kEnemyMaxNum = 4;
}
class BattleEnemyManager
{
public:
	struct Enemy
	{
		std::shared_ptr<GearFrame> _gearFrame;
		eMusical _type;
	};
public:
	BattleEnemyManager();
	~BattleEnemyManager();
	/*
		初期化時の引数でどの敵を配置するかを取得する
		_typeがNullの場合は初期化の処理を飛ばす
	*/
	void Initialize(std::array<Enemy, kEnemyMaxNum> enemyList);

	/*
		どの音を流すかを考えさせ
		音をリストにスタックするときに対象の音のenumを返す
		それ以外はeMusical::Nullを返す
	*/
	eMusical mUpadate(const float timeScale);

	/*
		描画処理
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);

	/*
		敵が全滅時にtrueを返すようにする
	*/
	bool mIsEnd();

private:
	std::array<BattleEnemy, kEnemyMaxNum> m_enemyArray;
	bool m_isEnd;
};

#endif