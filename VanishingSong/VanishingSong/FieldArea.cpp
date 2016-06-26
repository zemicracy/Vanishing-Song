#include "FieldArea.h"
#include<Rectangle3D.h>
#include<Cube.h>
#include<WorldReader.h>
#include <Physics.h>
#include "ResourceManager.h"
#include "GameManager.h"
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

	if (m_pNote){
		m_pNote->Finalize();
		m_pNote.reset();
		m_pNote = nullptr;
	}

	if (m_pGround){
		m_pGround->Finalize();
		m_pGround.reset();
		m_pGround = nullptr;
	}

	for (auto& itr : m_pObject){
		itr->Finalize();
		itr.reset();
	}
	for (auto& itr : m_wall){
		itr->Finalize();
		itr.reset();
	}
	for (auto& itr : m_partitionCube){
		itr->Finalize();
		itr.reset();
	}
	if (m_rhythmManager){
		m_rhythmManager->mFinalize();
	
	}
}

template<typename T>
void gInitalizer(std::shared_ptr<ModelBase> &command, Transform transform, Color color){
	command = std::make_shared<T>();
	command->Initialize();
	command->property._transform = transform;
	command->property._color = color;
}

void gSphereInitializer(std::shared_ptr<Sphere> &command, Transform transform, Color color){
	command = std::make_shared<Sphere>(10,10);
	command->Initialize();
	command->property._transform = transform;
	command->property._color = color;
}

void FieldArea::mInitialize(std::string texdirectory){
	WorldReader reader;
	reader.Load("data\\Field\\stage", true);

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "stage"){
			m_pGround = std::make_shared<FbxModel>();
			m_pGround->LoadFBX("Model\\Field\\Stage.fbx", eAxisSystem::eAxisOpenGL);
			m_pGround->SetTextureDirectoryName(texdirectory);
			m_pGround->property._transform._translation = itr->_transform._translation;
			m_pGround->property._transform._scale._x = -1;
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

	mInitializeObject();

	m_pNote = std::make_shared<FbxModel>();
	m_pNote->LoadFBX("Model\\Object\\Note\\note1.fbx", eAxisSystem::eAxisOpenGL);
	m_pNote->SetTextureDirectoryName("Model\\Object\\Note\\tex");
	m_pNote->property._transform._translation._y = 230;
	m_pNote->property._transform._scale = 25;

	m_skybox = std::make_shared<Skybox>();
	m_skybox->Initialize();
	m_skybox->SetTexture(ResourceManager::mGetInstance().GetTexture("skybox").get());

	for (auto& index : GameManager::mGetInstance().mGetUsePlayer()){
		m_usePlayer.push_back(index.first);
	}
	m_isChangeInit = false;
	m_isBlack = false;
	m_changeColorCount = NULL;

	m_originColor = Color(0, 0, 0, 1);
}

void FieldArea::mSetCamera(aetherClass::ViewCamera* camera){
	m_pGround->SetCamera(camera);
	m_skybox->SetCamera(camera);
	for (auto& itr : m_wall){
		itr->SetCamera(camera);
	}

	for (auto& itr : m_pObject){
		itr->SetCamera(camera);
	}

	m_pNote->SetCamera(camera);
}


void FieldArea::mRender(ShaderBase* texture, ShaderBase*shader){
	m_pGround->Render(texture);
	m_skybox->Render(texture);
	m_pNote->Render(shader);
}

void FieldArea::mUpdate(float time){

	if (m_rhythmManager){
		m_rhythmManager->mAcquire();
		if (GameManager::mGetInstance().mBossState() == GameManager::eBossState::eWin){
			
			float note = 360 * m_rhythmManager->mQuarterBeatTime();
			float nowFrameWave = cos(note * kAetherRadian);
			float scale = nowFrameWave >= 0.8 ? nowFrameWave : 0;

			float rote = 90 * m_rhythmManager->mQuarterBeatTime();

			m_pNote->property._transform._scale = 25 + (scale * 5);
			m_pNote->property._transform._rotation._y = rote;
		}
		else{
			if (m_pNote->property._transform._rotation._y > 360){
				m_pNote->property._transform._rotation._y -= 360;
			}
			m_pNote->property._transform._rotation._y += time * 1;
		}
	}
	
	mChangeColor();
	
}

std::shared_ptr<aetherClass::ModelBase>& FieldArea::mGetPartitionCube(const int number){
	return m_partitionCube[number];
}


std::array<std::shared_ptr<aetherClass::ModelBase>, 4>& FieldArea::mGetWallList(){
	return m_wall;
}

std::array<std::shared_ptr<Sphere>, 4>& FieldArea::mGetObjectList(){
	return m_pObject;
}

void FieldArea::mInitializeObject(){

	Transform trans;
	trans._scale = 105;
	gSphereInitializer(m_pObject[0], trans, Color(1, 0, 0, 0.3));
	trans._translation = Vector3(300, 0, 400);
	trans._scale = Vector3(65, 50, 65);
	gSphereInitializer(m_pObject[1], trans, Color(1, 0, 0, 0.3));
	trans._translation = Vector3(-200, 0, 500);
	gSphereInitializer(m_pObject[2], trans, Color(1, 0, 0, 0.3));
	trans._translation = Vector3(-200, 0, -285);
	gSphereInitializer(m_pObject[3], trans, Color(1, 0, 0, 0.3));
}

//
void FieldArea::mChangeColor(){
	if (!m_isChangeInit){
		mResizeUsePlayer();
		if (m_changeColorCount > m_usePlayer.size()-1){
			m_changeColorCount = 0;
		}
		if (!m_usePlayer.empty()){
			m_changeColor = mMusicalToColor(m_usePlayer.at(m_changeColorCount));
		}
		else{
			m_changeColor = Color(0, 0, 0, 1);
		}
		m_isChangeInit = true;
	}
	const float time = GameClock::GetDeltaTime();

	if (m_isBlack){
		m_originColor._red -= time*m_changeColor._red;
		m_originColor._green -= time*m_changeColor._green;
		m_originColor._blue -= time*m_changeColor._blue;
		if (m_originColor._red <= 0.0f&&m_originColor._blue <= 0.0f
			&&m_originColor._green <= 0.0f){
			m_isChangeInit = false;
			m_isBlack = false;
			m_changeColorCount += 1;
			m_originColor = Color(0, 0, 0, 1);
		}
	}
	else{
		m_originColor._red += time*m_changeColor._red;
		m_originColor._green += time*m_changeColor._green;
		m_originColor._blue += time*m_changeColor._blue;
		if (m_originColor._red >= m_changeColor._red&&m_originColor._blue >= m_changeColor._blue
			&&m_originColor._green >= m_changeColor._green){
			m_isBlack = true;
		}
	}
	m_pNote->SetModelColor(m_originColor);
	return;
}

Color FieldArea::mMusicalToColor(eMusical type){
	switch (type)
	{
	case eMusical::eBlue:
		return Color(0, 0, 1, 1);
		break;
	case eMusical::eGreen:
		return Color(0, 1, 0, 1);
	case eMusical::eRed:
		return Color(1, 0, 0, 1);
		
	case eMusical::eYellow:
		return Color(1, 1, 0, 1);
	default:
		break;
	}

	return Color(0, 0, 0, 1);
}

//
void FieldArea::mSetRhythm(RhythmManager* rhythm){
	m_rhythmManager = rhythm;
}

void FieldArea::mResizeUsePlayer(){
	if (m_usePlayer.size() == GameManager::mGetInstance().mGetUsePlayer().size())return;
	for (auto& index : GameManager::mGetInstance().mGetUsePlayer()){
		for (auto& pushIndex : m_usePlayer){
			if (pushIndex == index.first){
				continue;
			}
		}
		m_usePlayer.push_back(index.first);
	}

}