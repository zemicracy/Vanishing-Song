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
	void mUpdate(float scaleTime,aetherClass::Vector3);
private:
	std::array<std::shared_ptr<Cage>, 3> m_pCage;
};

