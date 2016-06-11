#ifndef _BATTLEPLAYER_H
#define _BATTLEPLAYER_H
#include "Const.h"
#include "CharaEntity.h"
#include <ShaderBase.h>
#include <FbxModel.h>
#include <unordered_map>
class BattlePlayer
{
	enum class eState{
		eWait,
		eDamage,
		eAttack,
		eNull
	};
public:
	BattlePlayer();
	~BattlePlayer();

	void mInitialize(eMusical,aetherClass::Vector3 position,std::shared_ptr<aetherClass::FbxModel> gearframe);
	eMusical mGetType();
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(const float);
	void mSetPosition(aetherClass::Vector3);
private:
	eMusical m_type;
	CharaEntity m_charaEntity;
	std::shared_ptr<aetherClass::FbxModel> m_model;
	aetherClass::Transform m_transform;
	eState m_prevState;
};

#endif