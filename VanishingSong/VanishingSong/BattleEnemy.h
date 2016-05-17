#ifndef _BATTLEENEMY_H
#define _BATTLEENEMY_H
#include "Const.h"
#include "GearFrame.h"
#include "ShaderBase.h"
#include <memory>
class BattleEnemy
{
public:
	BattleEnemy();
	~BattleEnemy();

	/*
		初期化処理
	*/
	void mInitialize(eMusical type, std::shared_ptr<GearFrame>);
	eMusical mGetType();

	/*
		更新処理
		基本的に待機モーションを行う
	*/
	void mUpDate(const float timeScale);

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
private:
	eMusical m_type;
	std::shared_ptr<GearFrame> m_GearFrame;
};

#endif