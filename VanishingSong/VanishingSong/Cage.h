#ifndef _CAGE_H
#define _CAGE_H
#include "Animation.h"
#include <ShaderBase.h>
#include "GearFrame.h"
#include "CharaEntity.h"

class Cage
{
public:
	Cage(std::shared_ptr<GearFrame>, aetherClass::Vector3 position, aetherClass::ViewCamera*);
	~Cage();

	void mInitialize(std::shared_ptr<GearFrame>, aetherClass::Vector3 position, aetherClass::ViewCamera*);
	void mUpdate(const float timeScale);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mFinalize();

	std::shared_ptr < aetherClass::Cube> mGetCollider();
private:

	void mPrisonerAnimation();

	void mCreatePrisoner();
	void mLoadModel(std::shared_ptr<GearFrame>&, std::string modelDataFile);

	void mRegisterGearHash(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, std::shared_ptr<GearFrame>&);
	/*
	アニメーションの登録
	*/
	void mRegisterAnimation(const int allFrame, std::string first, std::string last);
private:
	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_gearHash;   // それぞれのギアのポインタを扱いやすいようにまとめた連想配列
	std::shared_ptr<Gear> m_topGear;            // 最上位パーツのポインタを入れておく
	std::shared_ptr<aetherClass::FbxModel> m_cage;
	AnimationFrame m_stayAnimation;
	aetherClass::Vector3 m_initialPosition;
	CharaEntity m_charaEntity;
	std::shared_ptr < aetherClass::Cube> m_pCollider;
	int m_animationCounter;
	bool m_changeAnimation;
};

#endif