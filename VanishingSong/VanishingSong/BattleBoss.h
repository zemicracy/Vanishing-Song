#ifndef _BATTLEBOSS_H
#define _BATTLEBOSS_H
#include "Const.h"
#include "ViewCamera.h"
#include "ShaderBase.h"
#include<FbxModel.h>
#include <memory>

class BattleBoss
{
public:
	BattleBoss();
	~BattleBoss();

	void mInitialize(eMusical type, aetherClass::ViewCamera*, aetherClass::Vector3&);
	void mUpdate(const float timeScale);
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);
	void mFinalize();

private:
	std::shared_ptr<aetherClass::FbxModel> m_pModel;
	bool m_bossFlag;

};

#endif