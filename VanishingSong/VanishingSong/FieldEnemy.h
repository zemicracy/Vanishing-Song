#ifndef _FieldEnemy_H
#define _FieldEnemy_H
#include "CharaStatus.h"
#include "GameController.h"
#include "GearFrame.h"
#include "Cube.h"
#include "CharaEntity.h"
#include <array>

class FieldEnemy
{
public:

	struct Property{
		std::shared_ptr<GearFrame> _penemy;			//Enemy本体
		std::shared_ptr<aetherClass::Cube> _pCollider;	//コライダー
		int _enemyAreaNo;		//エリアナンバー
	};

	FieldEnemy();
	~FieldEnemy();

	bool mInitializeGround(aetherClass::ViewCamera*);				//敵初期化(地上)
	bool mInitializeAir(aetherClass::ViewCamera*);				//敵初期化(空中)
	void mInitializeEnemyColider(aetherClass::ViewCamera*);	//コライダーの初期化
	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mFinalize();	//開放処理
	void mEnemyOnHit();	//当たったかどうか
	Property &mGetProperty();

private:
	CharaEntity m_charaEntity;
	std::shared_ptr<Gear> m_pTopGear;
	Property m_property;
};
#endif