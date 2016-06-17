#pragma once
#include "Cage.h"
#include "FieldEnemyManager.h"
#include <array>
class CageManager
{
public:
	CageManager();
	~CageManager();

	void mInitialize(FieldEnemyManager*,aetherClass::ViewCamera*);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUIRender(aetherClass::ShaderBase*);
	void mUpdate(float scaleTime,aetherClass::Vector3,const bool);
	aetherClass::Vector3 mGetPosition(const int);
	void mSetIsComment(const int,const bool);
	bool mGetIsMessageRun(const int);
	std::shared_ptr<aetherClass::Cube>& mGetColldier(const int);
private:
	std::array<std::shared_ptr<Cage>, 3> m_pCage;
};

