#ifndef _BATTLEENEMY_H
#define _BATTLEENEMY_H
#include "Const.h"
#include "ActionBlue.h"
#include "ShaderBase.h"
#include "CharaEntity.h"
#include <memory>
#include<FbxModel.h>
class BattleEnemy
{

public:
	struct Enemy
	{
		std::shared_ptr<aetherClass::FbxModel> _model;
		eMusical _type;
	};

public:
	BattleEnemy();
	~BattleEnemy();

	/*
		初期化処理
	*/
	void mInitialize(eMusical type,eEnemyType enemytype, aetherClass::ViewCamera*,aetherClass::Vector3&);

	Enemy &mGetEnemy();
	eMusical mGetType();

	/*
		更新処理
		基本的に待機モーションを行う
	*/
	void mUpdate(const float timeScale);

	/*
		攻撃のアニメーションの再生
	*/
	void mOnAttack();

	/*
		ダメージのアニメーションの再生
	*/
	void mOnDamage();

	void misDie();

	
	/*
		描画処理
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);
private:
	// 解放処理
	// ただしGearFrameは解放しない
	void Finalize();


private:
	eMusical m_type;
	Enemy m_enemy;
	bool m_isDie;
	float m_alpha;
};

#endif