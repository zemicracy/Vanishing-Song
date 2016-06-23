#pragma once
#include"Skybox.h"
#include <FbxModel.h>
#include<array>
#include<memory>
#include<unordered_map>
#include <Sphere.h>
#include "RhythmManager.h"
#include "Const.h"
class FieldArea
{
public:
	void mInitialize(std::string texdirectory);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdate(float);
	std::shared_ptr<aetherClass::ModelBase>& mGetPartitionCube(const int number);
	std::array<std::shared_ptr<aetherClass::ModelBase>, 4>& mGetWallList();
	std::array<std::shared_ptr<aetherClass::Sphere>, 4>& mGetObjectList();
	void mSetCamera(aetherClass::ViewCamera*);
	void mSetRhythm(RhythmManager*);
	FieldArea();
	~FieldArea();
private:
	void mInitializeObject();
	void mFinalize();
	void mChangeColor();
	aetherClass::Color mMusicalToColor(eMusical);
	std::array<std::shared_ptr<aetherClass::ModelBase>, 4>m_wall;
	std::array<std::shared_ptr<aetherClass::ModelBase>, 5>m_partitionCube;
	std::shared_ptr<aetherClass::ModelBase> m_skybox;
	std::shared_ptr<aetherClass::FbxModel> m_pGround;
	std::array<std::shared_ptr<aetherClass::Sphere>, 4> m_pObject;
	std::shared_ptr<aetherClass::FbxModel> m_pNote;
	RhythmManager* m_rhythmManager;
	aetherClass::Color m_changeColor;
	aetherClass::Color m_originColor;
	std::vector<eMusical> m_usePlayer;
	bool m_isChangeInit;
	bool m_isBlack;
	int m_changeColorCount;
};

