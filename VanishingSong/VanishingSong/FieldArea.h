#pragma once
#include"Skybox.h"
#include<array>
#include<memory>
#include<unordered_map>
class FieldArea
{
public:
	void mInitialize();
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(float);
	std::shared_ptr<aetherClass::ModelBase> mGetPartitionCube(const int number);
	void mSetCamera(aetherClass::ViewCamera*);
	FieldArea();
	~FieldArea();
private:
	void mFinalize();
	std::array<std::shared_ptr<aetherClass::ModelBase>, 4>m_wall;
	std::array<std::shared_ptr<aetherClass::ModelBase>, 4>m_partitionCube;
	std::array<std::shared_ptr<aetherClass::ModelBase>, 2>m_ground;

};

