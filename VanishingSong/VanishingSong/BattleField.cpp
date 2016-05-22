#include "BattleField.h"
#include<WorldReader.h>
#include<Skybox.h>
#include<Rectangle3D.h>
#include<Cube.h>
#include<Singleton.h>
#include"ResourceManager.h"

BattleField::BattleField()
{
}


BattleField::~BattleField()
{
	mFinalize();
}

void BattleField::mFinalize(){
	for (auto itr : m_pLane){
		itr.second->Finalize();
	}
	m_pLane.clear();

	for (auto itr : m_pGeneral){
		itr->Finalize();
		itr.reset();
	}
	for (auto itr : m_pDebug){
		itr->Finalize();
		itr.reset();
	}
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

void BattleField::mInitialize(aetherClass::ViewCamera* camera){
	using namespace aetherClass;

	WorldReader reader;
	reader.Load("data\\BattleField.aether");

	camera->property._translation = reader.GetInputWorldInfo()._camera._position;
	camera->property._rotation = reader.GetInputWorldInfo()._camera._rotation;
	m_view = camera;

	Color WHITE(0, 0, 0, 0.7);
	Color YELLOW(1, 1, 1, 0);
	float normalScale = 0.9;

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "floor"){
			m_pGeneral[0] = gInitializer<Rectangle3D>(itr->_transform, itr->_color);
		}
		else if (itr->_name == "playerside"){
			m_pDebug[0] = gInitializer<Cube>(itr->_transform, itr->_color);
		}
		else if (itr->_name == "enemyside"){
			m_pDebug[1] = gInitializer<Cube>(itr->_transform, itr->_color);
		}
		else if (itr->_name == "lane1"){
			m_pLane.insert(std::make_pair(eMusical::eBlue, gInitializer<Rectangle3D>(itr->_transform, YELLOW)));
			m_command[0] = gInitializer<Rectangle3D>(itr->_transform, WHITE);
			m_command[0]->property._transform._scale = Vector3(normalScale, normalScale, 1);
			m_command[0]->property._transform._translation._y += 1;
			m_command[0]->property._transform._rotation = m_view->property._rotation;
			m_command[0]->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture("ActionBlue").get());

		}
		else if (itr->_name == "lane2"){
			m_pLane.insert(std::make_pair(eMusical::eGreen, gInitializer<Rectangle3D>(itr->_transform, YELLOW)));
			m_command[1] = gInitializer<Rectangle3D>(itr->_transform, WHITE);
			m_command[1]->property._transform._scale = Vector3(normalScale, normalScale, 1);
			m_command[1]->property._transform._translation._y += 1;
			m_command[1]->property._transform._rotation = m_view->property._rotation;
			m_command[1]->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture("ActionGreen").get());
		}
		else if (itr->_name == "lane3"){
			m_pLane.insert(std::make_pair(eMusical::eRed, gInitializer<Rectangle3D>(itr->_transform, YELLOW)));
			m_command[2] = gInitializer<Rectangle3D>(itr->_transform, WHITE);
			m_command[2]->property._transform._scale = Vector3(normalScale, normalScale, 1);
			m_command[2]->property._transform._translation._y += 1;
			m_command[2]->property._transform._rotation = m_view->property._rotation;
			m_command[2]->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture("ActionRed").get());
		}
		else if (itr->_name == "lane4"){
			m_pLane.insert(std::make_pair(eMusical::eYellow, gInitializer<Rectangle3D>(itr->_transform, YELLOW)));
			m_command[3] = gInitializer<Rectangle3D>(itr->_transform, WHITE);
			m_command[3]->property._transform._scale = Vector3(normalScale, normalScale, 1);
			m_command[3]->property._transform._translation._y += 1;
			m_command[3]->property._transform._rotation = m_view->property._rotation;
			m_command[3]->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture("ActionYellow").get());
		}
	}

	m_pGeneral[1] = std::make_shared<Skybox>();
	m_pGeneral[1]->Initialize();
	m_pTextureList.insert(std::make_pair("skybox", gCreateTexture("Texture\\Game\\GameBack.jpg")));
	m_pGeneral[1]->SetTexture(m_pTextureList["skybox"].get());

	for (auto itr : m_pDebug){
		itr->SetCamera(m_view);
	}
	for (auto itr : m_pLane){
		itr.second->SetCamera(m_view);
	}
	for (auto itr : m_pGeneral){
		itr->SetCamera(m_view);
	}
	for (auto itr : m_command){
		itr->SetCamera(m_view);
	}
}

void BattleField::mUpdate(std::shared_ptr<ActionCommand>command){
	//ƒ¿‚¯‚·‚æ
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
	m_pGeneral[0]->Render(debug);
	m_pGeneral[1]->Render(texture);

	for (auto itr : m_pLane){
		itr.second->Render(debug);
	}
	for (auto itr : m_pDebug){
		itr->Render(debug);
	}
	for (int i = m_command.size()-1; i >= 0; --i){
		m_command.at(i)->Render(texture);
	}

}
