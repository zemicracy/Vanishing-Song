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
	enum class eType{
		Air,
		Ground,
		Blue,
		Yellow,
		Null
	};

	struct Property{
		std::shared_ptr<GearFrame> _penemy;			//Enemy本体
		std::shared_ptr<aetherClass::ModelBase> _pCollider;	//コライダー
		bool _talkflag;
		int _enemyAreaNo;		//エリアナンバー
	};

	FieldEnemy();
	~FieldEnemy();

	bool mInitialize(eType,aetherClass::ViewCamera*,std::string dataPath);
	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mFinalize();	//開放処理
	void mEnemyOnHit();	//当たったかどうか
	Property &mGetProperty();
	void mFaceToPlayer(aetherClass::Vector3);
	void mRegisterMessage(std::string);
	int mGetMessageNum()const;
	std::shared_ptr<aetherClass::Texture> mGetMessage(const int id);
	std::string mGetBattleDataPath();
private:
	bool mInitializeGround(aetherClass::ViewCamera*);				//敵初期化(地上)
	bool mInitializeAir(aetherClass::ViewCamera*);				//敵初期化(空中)
	bool mInitializeYellow(aetherClass::ViewCamera*);				//敵初期化(地上)
	bool mInitializeBlue(aetherClass::ViewCamera*);				//敵初期化(空中)
	void mInitializeEnemyColider(aetherClass::ViewCamera*);	//コライダーの初期化

	CharaEntity m_charaEntity;
	std::shared_ptr<Gear> m_pTopGear;
	Property m_property;
	std::vector<std::shared_ptr<aetherClass::Texture>> m_message;
	std::string m_dataPath;
};
#endif