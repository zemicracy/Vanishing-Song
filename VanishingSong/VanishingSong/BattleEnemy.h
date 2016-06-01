#ifndef _BATTLEENEMY_H
#define _BATTLEENEMY_H
#include "Const.h"
#include "GearFrame.h"
#include "ActionBlue.h"
#include "ShaderBase.h"
#include "CharaEntity.h"
#include <memory>
class BattleEnemy
{

public:
	struct Enemy
	{
		std::shared_ptr<GearFrame> _gearFrame;
		eMusical _type;
	};

public:
	BattleEnemy();
	~BattleEnemy();

	/*
		初期化処理
	*/
	void mInitialize(eMusical type, aetherClass::ViewCamera*,aetherClass::Vector3&);

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

	
	/*
		描画処理
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);
private:
	// 解放処理
	// ただしGearFrameは解放しない
	void Finalize();
	bool mInitializeBlue(aetherClass::ViewCamera*, aetherClass::Vector3& pos);
	bool mInitializeGreen(aetherClass::ViewCamera*, aetherClass::Vector3& pos);
	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);


private:
	eMusical m_type;
	Enemy m_enemy;
	CharaEntity m_charaEntity;;
	std::shared_ptr<GearFrame> m_GearFrame;
	std::shared_ptr<Gear> m_pTopGear;

};

#endif