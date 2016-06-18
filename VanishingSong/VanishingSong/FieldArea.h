#pragma once
#include"Skybox.h"
#include <FbxModel.h>
#include<array>
#include<memory>
#include<unordered_map>
#include <Sphere.h>
class FieldArea
{
public:
	void mInitialize();
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdate(float);
	std::shared_ptr<aetherClass::ModelBase>& mGetPartitionCube(const int number);
	std::array<std::shared_ptr<aetherClass::ModelBase>, 2>& mGetPartitionWall(const int number);
	std::array<std::shared_ptr<aetherClass::Sphere>, 4>& mGetObjectList();
	void mSetCamera(aetherClass::ViewCamera*);
	FieldArea();
	~FieldArea();
private:
	void mInitializeObject();
	void mFinalize();
	std::array<std::shared_ptr<aetherClass::ModelBase>, 4>m_wall;
	std::array<std::shared_ptr<aetherClass::ModelBase>, 5>m_partitionCube;
	std::shared_ptr<aetherClass::ModelBase> m_skybox;
	std::shared_ptr<aetherClass::FbxModel> m_pGround;
	std::array<std::array<std::shared_ptr<aetherClass::ModelBase>, 2>, 4> m_partitionWall;
	std::array<std::shared_ptr<aetherClass::Sphere>, 4> m_pObject;
	std::shared_ptr<aetherClass::FbxModel> m_pObejctNote;
};

