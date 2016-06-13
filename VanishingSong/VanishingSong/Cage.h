#ifndef _CAGE_H
#define _CAGE_H
#include <ShaderBase.h>
#include <Cube.h>
#include "CharaEntity.h"

class Cage
{
public:
	Cage(std::shared_ptr<aetherClass::FbxModel>, aetherClass::Vector3 position, aetherClass::ViewCamera*, bool);
	~Cage();

	void mUpdate(const float timeScale,aetherClass::Vector3);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mSetIsTought(bool);
	std::shared_ptr < aetherClass::Cube> mGetCollider();
private:

	void mInitialize(std::shared_ptr<aetherClass::FbxModel>, aetherClass::Vector3 position, aetherClass::ViewCamera*,bool);
	void mFinalize();

	void mCreatePrisoner();
private:
	std::shared_ptr<aetherClass::FbxModel> m_model;
	std::shared_ptr<aetherClass::FbxModel> m_cage;
	aetherClass::Vector3 m_initialPosition;
	CharaEntity m_charaEntity;
	std::shared_ptr < aetherClass::Cube> m_pCollider;
	bool m_isTought;
};

#endif