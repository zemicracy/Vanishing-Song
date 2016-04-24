#include "FieldArea.h"
#include<Rectangle3D.h>
#include<Cube.h>
#include<WorldReader.h>

using namespace aetherClass;
FieldArea::FieldArea()
{

}


FieldArea::~FieldArea()
{
	mFinalize();
}
void FieldArea::mFinalize(){
	for (auto itr : m_ground){
		itr->Finalize();
		itr.reset();
	}
	for (auto itr : m_wall){
		itr->Finalize();
		itr.reset();
	}
	for (auto itr : m_partitionCube){
		itr->Finalize();
		itr.reset();
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
	reader.Load("data/stage.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "stage"){
			gInitalizer<Rectangle3D>(m_ground[0], itr->_transform, itr->_color);
		}
		if (itr->_name == "wall1"){
			gInitalizer<Rectangle3D>(m_wall[0], itr->_transform, itr->_color);
		}
		if (itr->_name == "wall2"){
			gInitalizer<Rectangle3D>(m_wall[1], itr->_transform, itr->_color);
		}
		if (itr->_name == "wall3"){
			gInitalizer<Rectangle3D>(m_wall[2], itr->_transform, itr->_color);
		}
		if (itr->_name == "wall4"){
			gInitalizer<Rectangle3D>(m_wall[3], itr->_transform, itr->_color);
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
	}
	reader.UnLoad();

	m_ground[1] = std::make_shared<Skybox>();
	m_ground[1]->Initialize();
	

}

void FieldArea::mSetCamera(aetherClass::ViewCamera* camera){
	for (auto itr : m_ground){
		itr->SetCamera(camera);
	}
	for (auto itr : m_wall){
		itr->SetCamera(camera);
	}

}


void FieldArea::mRender(aetherClass::ShaderBase* shader){
	for (auto itr : m_ground){
		itr->Render(shader);
	}
	for (auto itr : m_wall){
		itr->Render(shader);
	}
}

void FieldArea::mUpdate(float){
		
}

std::shared_ptr<aetherClass::ModelBase> FieldArea::mGetPartitionCube(const int number){
	return m_partitionCube[number];
}
