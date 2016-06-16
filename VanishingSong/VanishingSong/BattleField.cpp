#include "BattleField.h"
#include<WorldReader.h>
#include<Skybox.h>
#include<Rectangle3D.h>
#include<Cube.h>
#include<Singleton.h>
#include"ResourceManager.h"
#include"GameController.h"

BattleField::BattleField()
{
}


BattleField::~BattleField()
{
	mFinalize();
}

void BattleField::mFinalize(){
	for (auto& itr : m_pLane){
		itr.second->Finalize();
	}
	m_pLane.clear();

	m_pSkyBox->Finalize();
	m_pPlane->Finalize();
	m_pTank->Finalize();

	for (auto& itr : m_pCommand){
		itr->Finalize();
		itr.reset();
	}

	m_EnemyLane.clear();
	m_PlayerLane.clear();
	for (auto& itr : m_pTextureList){
		itr.second.reset();
	}
	m_pTextureList.clear();
}
template<class T>
std::shared_ptr<T> gInitializer(aetherClass::Transform transform, aetherClass::Color color){
	auto obj = std::make_shared<T>();
	obj->Initialize();
	obj->property._transform = transform;
	obj->property._color = color;
	return obj;
}

std::shared_ptr<aetherClass::Texture> gCreateTexture(std::string path){
	auto tex = std::make_shared<aetherClass::Texture>();
	tex->Load(path);
	return tex;
}

void BattleField::mInitialize(aetherClass::ViewCamera* camera,RhythmManager *rhythm,bool boss){
	using namespace aetherClass;

	WorldReader reader;
	reader.Load("data\\BattleStage.aether");

	m_rhythm = rhythm;
	m_isBossStage = boss;
	camera->property._translation = reader.GetInputWorldInfo()._camera._position;
	camera->property._rotation = reader.GetInputWorldInfo()._camera._rotation;
	m_view = camera;

	Color COMMAND(0, 0, 0, 0.8);
	Color WHITE(1, 1, 1, 0);
	float normalScale = 9.9;

	for (auto &itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "stage"){
			m_pPlane = std::make_shared<FbxModel>();
			m_pPlane->LoadFBX("Model\\BattleStage.fbx", aetherClass::eAxisSystem::eAxisOpenGL);
			m_pPlane->SetCamera(m_view);
			m_pPlane->property._transform = itr->_transform;
			m_pPlane->property._transform._scale._z = 2;
			//m_pPlane->SetTexture()
		}
		else if (itr->_name == "PlayerHP"){
			m_tankScaleOrigin = 1.8;
		}
		else if (itr->_name == "EnemyHP"){
			m_pTank	= std::make_shared<FbxModel>();
			m_pTank->LoadFBX("Model\\Battle\\capsule.fbx", aetherClass::eAxisSystem::eAxisOpenGL);
			m_pTank->SetTextureDirectoryName("Model\\Battle");
			m_pTank->SetCamera(m_view);
			m_pTank->property._transform = itr->_transform;
			m_pTank->property._transform._rotation._y = -90;
			m_pTank->property._transform._scale = 1;
			m_EnemyLane[eMusical::eAdlib] = itr->_transform._translation;
		}
		else if (itr->_name == "lane_blue"){
			m_pLane.insert(std::make_pair(eMusical::eBlue, gInitializer<Rectangle3D>(itr->_transform, WHITE)));
		}
		else if (itr->_name == "command_blue"){
			m_pCommand[0] = gInitializer<Rectangle3D>(itr->_transform, COMMAND);
			m_pCommand[0]->property._transform._rotation = m_view->property._rotation;
			m_pCommand[0]->SetTexture(ResourceManager::mGetInstance().GetTexture("ActionBlue").get());
		}
		else if (itr->_name == "lane_green"){
			m_pLane.insert(std::make_pair(eMusical::eGreen, gInitializer<Rectangle3D>(itr->_transform, WHITE)));
		}
		else if (itr->_name == "command_green"){
			m_commandScale = itr->_transform._scale._x;

			m_pCommand[1] = gInitializer<Rectangle3D>(itr->_transform, COMMAND);
			m_pCommand[1]->property._transform._rotation = m_view->property._rotation;
			m_pCommand[1]->SetTexture(ResourceManager::mGetInstance().GetTexture("ActionGreen").get());
		}
		else if (itr->_name == "lane_red"){
			m_pLane.insert(std::make_pair(eMusical::eRed, gInitializer<Rectangle3D>(itr->_transform, WHITE)));
		}
		else if (itr->_name == "command_red"){
			m_pCommand[2] = gInitializer<Rectangle3D>(itr->_transform, COMMAND);
			m_pCommand[2]->property._transform._rotation = m_view->property._rotation;
			m_pCommand[2]->SetTexture(ResourceManager::mGetInstance().GetTexture("ActionRed").get());
		}
		else if (itr->_name == "lane_yellow"){
			m_pLane.insert(std::make_pair(eMusical::eYellow, gInitializer<Rectangle3D>(itr->_transform, WHITE)));
		}
		else if (itr->_name == "command_yellow"){
			m_pCommand[3] = gInitializer<Rectangle3D>(itr->_transform, COMMAND);
			m_pCommand[3]->property._transform._rotation = m_view->property._rotation;
			m_pCommand[3]->SetTexture(ResourceManager::mGetInstance().GetTexture("ActionYellow").get());

		}
		else if (itr->_name == "enemy_blue"){
			m_EnemyLane[eMusical::eBlue] = itr->_transform._translation;
		}
		else if (itr->_name == "enemy_green"){
			m_EnemyLane[eMusical::eGreen] = itr->_transform._translation;
		}
		else if (itr->_name == "enemy_red"){
			m_EnemyLane[eMusical::eRed] = itr->_transform._translation;
		}
		else if (itr->_name == "enemy_yellow"){
			m_EnemyLane[eMusical::eYellow] = itr->_transform._translation;
		}
		else if (itr->_name == "player_blue"){
			m_PlayerLane[eMusical::eBlue] = itr->_transform._translation;
		}
		else if (itr->_name == "player_green"){
			m_PlayerLane[eMusical::eGreen] = itr->_transform._translation;
		}
		else if (itr->_name == "player_red"){
			m_PlayerLane[eMusical::eRed] = itr->_transform._translation;
		}
		else if (itr->_name == "player_yellow"){
			m_PlayerLane[eMusical::eYellow] = itr->_transform._translation;
		}
	}

	m_pSkyBox = std::make_shared<Skybox>();
	m_pSkyBox->Initialize();
	m_pTextureList.insert(std::make_pair("skybox", ResourceManager::mGetInstance().GetTexture("skybox")));
	m_pSkyBox->SetTexture(m_pTextureList["skybox"].get());

	for (auto itr : m_pLane){
		itr.second->SetCamera(m_view);
	}
	for (auto itr : m_pCommand){
		itr->SetCamera(m_view);
	}
	m_pSkyBox->SetCamera(m_view);
	m_pSkyBox->property._transform._rotation._x = 22.7f;

	reader.UnLoad();
}

void BattleField::mUpdate(std::shared_ptr<ActionCommand>command){
	//ƒ¿‚¯‚·—p
	for (auto itr : m_pLane){
		if (itr.second->property._color._alpha > 0){
			itr.second->property._color._alpha -= 0.1;
		}
	}
	//‘Î‰ž‚µ‚½ƒŒ[ƒ“‚ð@ƒ¿‚P
	if (m_pLane.find(command->mGetType()) == m_pLane.end())return;
	m_pLane.at(command->mGetType())->property._color._alpha = 0.9;
}

void BattleField::mRender(aetherClass::ShaderBase *texture, aetherClass::ShaderBase *debug){
	m_pSkyBox->Render(texture);
	m_pPlane->Render(debug);

	for (auto itr : m_pLane){
		itr.second->Render(debug);
	}
	if (m_isBossStage){
		m_pTank->Render(texture);
	}
	for (int i = m_pCommand.size()-1; i >= 0; --i){
		m_pCommand.at(i)->Render(texture);
	}
}


aetherClass::Vector3 BattleField::mGetEnemyLane(eMusical type){
	return m_EnemyLane.at(type);
}

aetherClass::Vector3 BattleField::mGetPlayerLane(eMusical type){
	return m_PlayerLane.at(type);
}

aetherClass::ViewCamera* BattleField::mGetCamera(){
	return m_view;
}

void BattleField::mRhythmicMotion(){
	
	if (!m_rhythm)return;
	float note = 360 * m_rhythm->mQuarterBeatTime();
	float nowFrameWave = cos(note * kAetherRadian);
	float scale = nowFrameWave >= 0.8 ? nowFrameWave : 0;

	m_pTank->property._transform._scale = m_tankScaleOrigin + (scale / 10);

	for (auto itr : m_pCommand)
	{
		itr->property._transform._scale = m_commandScale + (2 * scale);
	}


}
