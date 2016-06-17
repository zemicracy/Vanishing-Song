#include "FieldArea.h"
#include<Rectangle3D.h>
#include<Cube.h>
#include<WorldReader.h>
#include <Physics.h>
#include "ResourceManager.h"
#include <Singleton.h>
using namespace aetherFunction;
using namespace aetherClass;
FieldArea::FieldArea()
{

}


FieldArea::~FieldArea()
{
	mFinalize();
}
void FieldArea::mFinalize(){
	if (m_skybox){
		m_skybox->Finalize();
		m_skybox.reset();
		m_skybox = nullptr;
	}

	if (m_pGround){
		m_pGround->Finalize();
		m_pGround.reset();
		m_pGround = nullptr;
	}

	for (auto& itr : m_wall){
		itr->Finalize();
		itr.reset();
	}
	for (auto& itr : m_partitionCube){
		itr->Finalize();
		itr.reset();
	}
	for (auto& itr : m_partitionWall){
		for (auto& itr2 : itr){
			itr2->Finalize();
			itr2.reset();
		}
	}

}

template<typename T>
void gInitalizer(std::shared_ptr<ModelBase> &command, Transform transform, Color color){
	command = std::make_shared<T>();
	command->Initialize();
	command->property._transform = transform;
	command->property._color = color;
}


void FieldArea::mInitialize(){
	WorldReader reader;
	reader.Load("data\\Field\\stage.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "stage"){
			m_pGround = std::make_shared<FbxModel>();
			m_pGround->LoadFBX("Model\\Field\\Stage.fbx", eAxisSystem::eAxisOpenGL);
			m_pGround->SetTextureDirectoryName("Model\\Field\\tex");
			m_pGround->property._transform = itr->_transform;
			m_pGround->property._transform._scale._x = -1;
			m_pGround->property._transform._rotation._x = 180;

		}
		if (itr->_name == "wall1"){
			gInitalizer<Cube>(m_wall[0], itr->_transform, itr->_color);
		}
		if (itr->_name == "wall2"){
			gInitalizer<Cube>(m_wall[1], itr->_transform, itr->_color);
		}
		if (itr->_name == "wall3"){
			gInitalizer<Cube>(m_wall[2], itr->_transform, itr->_color);
		}
		if (itr->_name == "wall4"){
			gInitalizer<Cube>(m_wall[3], itr->_transform, itr->_color);
		}

		if (itr->_name == "area1"){
			gInitalizer<Cube>(m_partitionCube[0], itr->_transform, itr->_color);
		}
		if (itr->_name == "area2"){
			gInitalizer<Cube>(m_partitionCube[1], itr->_transform, itr->_color);
		}
		if (itr->_name == "area3"){
			gInitalizer<Cube>(m_partitionCube[2], itr->_transform, itr->_color);
		}
		if (itr->_name == "area4"){
			gInitalizer<Cube>(m_partitionCube[3], itr->_transform, itr->_color);
		}
		if (itr->_name == "area5"){
			gInitalizer<Cube>(m_partitionCube[4], itr->_transform, itr->_color);
		}
	}
	reader.UnLoad();

	m_skybox = std::make_shared<Skybox>();
	m_skybox->Initialize();
	m_skybox->SetTexture(ResourceManager::mGetInstance().GetTexture("skybox").get());

	// 先にコライダーの検出をする
	int nextNumber = NULL;
	for (int i = 0; i < 4; ++i){
		for (auto& wall : m_partitionWall[i]){
			for (int j = nextNumber; j < 4; ++j){
				if (CollideBoxOBB(*m_wall[j], *m_partitionCube[i])){
					wall = m_wall[j];
					nextNumber = j + 1;
					break;
				}
			}
		}
		nextNumber = NULL;
	}
}

void FieldArea::mSetCamera(aetherClass::ViewCamera* camera){
	m_pGround->SetCamera(camera);
	m_skybox->SetCamera(camera);
	for (auto itr : m_wall){
		itr->SetCamera(camera);
	}
}


void FieldArea::mRender(aetherClass::ShaderBase* texture, aetherClass::ShaderBase*shader){
	m_pGround->Render(texture);
	m_skybox->Render(texture);
	for (auto itr : m_wall){
		itr->Render(shader);
	}
}

void FieldArea::mUpdate(float){
		
}

std::shared_ptr<aetherClass::ModelBase> FieldArea::mGetPartitionCube(const int number){
	return m_partitionCube[number];
}


std::array<std::shared_ptr<aetherClass::ModelBase>, 2>& FieldArea::mGetPartitionWall(const int number){
	return m_partitionWall[number];
}