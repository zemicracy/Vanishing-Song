#ifndef _BATTLEPLAYER_H
#define _BATTLEPLAYER_H
#include "Const.h"
#include "CharaEntity.h"
#include "GearFrame.h"
#include <ShaderBase.h>
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

	void mInitialize(eMusical,aetherClass::Vector3 position,std::shared_ptr<GearFrame> gearframe);
	eMusical mGetType();
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(const float);
	void mSetPosition(aetherClass::Vector3);
private:
	void mAnimation(eState state);
	void mRegisterAnimation(eState key, const int allFrame, std::string first, std::string last);
private:
	eMusical m_type;
	CharaEntity m_charaEntity;
	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_gearHash;   // それぞれのギアのポインタを扱いやすいようにまとめた連想配列
	std::shared_ptr<GearFrame> m_playerGear;
	aetherClass::Transform m_transform;
	std::unordered_map<eState, AnimationFrame> m_animationHash; 
	eState m_prevState;
	std::pair<int, bool> m_counter;
};

#endif