#ifndef _BATTLEPLAYER_H
#define _BATTLEPLAYER_H
#include "Const.h"
#include "CharaEntity.h"
#include <ShaderBase.h>
#include <FbxModel.h>
#include <unordered_map>
class BattlePlayer
{
public:
	enum class eBattleActionType{
		eWait, eAttack, eDamage,eWin
	};

public:
	BattlePlayer();
	~BattlePlayer();


	void mInitialize(eMusical, aetherClass::Vector3 position, std::shared_ptr<aetherClass::FbxModel> gearframe);
	eMusical mGetType();
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(const float);
	void mSetPosition(aetherClass::Vector3);
	void BattlePlayer::mChangeAnimation(std::string name);
private:
	std::string m_animationName;
	int m_animationFrame;
	int m_winAnimationFrame;
	eMusical m_type;
	CharaEntity m_charaEntity;
	std::shared_ptr<aetherClass::FbxModel> m_model;
	aetherClass::Transform m_transform;
	std::string m_prevAnimationName;
	bool m_isWin;
	bool m_change;
};

#endif