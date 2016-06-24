#include "SceneEnd.h"
#include "ResourceManager.h"
#include "GameController.h"

using namespace aetherClass;


const std::string SceneEnd::Name = "End";
SceneEnd::SceneEnd() :
GameScene(Name, GetManager())
{
}


SceneEnd::~SceneEnd()
{
}

bool SceneEnd::Initialize(){

	Debug::mPrint("yobareta");

	m_field.mInitialize("Model\\Field\\title_tex");
	m_field.mSetCamera(&m_view);

	
	m_pPlayer[0] = ResourceManager::mGetInstance().mGetPlayerHash(eMusical::eBlue);
	m_pPlayer[1] = ResourceManager::mGetInstance().mGetPlayerHash(eMusical::eRed);
	m_pPlayer[2] = ResourceManager::mGetInstance().mGetPlayerHash(eMusical::eGreen);
	m_pPlayer[3] = ResourceManager::mGetInstance().mGetPlayerHash(eMusical::eYellow);
	m_pBoss = ResourceManager::mGetInstance().mGetEnemyHash(eMusical::eAdlib);

	for (int i = 0; i < m_pPlayer.size(); i++){
		m_pPlayer[i]->SetCamera(&m_view);
	}
	
	m_pBoss->SetCamera(&m_view);

	WorldReader reader;
	reader.Load("data\\End\\PlayerEndScene.aether");

	for (auto index : reader.GetInputWorldInfo()._object){
		if (index->_name == "playerBlue"){
			m_pPlayer[0]->property._transform._translation = index->_transform._translation;
			m_pPlayer[0]->property._transform._rotation = 180;
		}
		if (index->_name == "playerRed"){
			m_pPlayer[1]->property._transform._translation = index->_transform._translation;
			m_pPlayer[1]->property._transform._rotation = 180;
		}
		if (index->_name == "playerGreen"){
			m_pPlayer[2]->property._transform._translation = index->_transform._translation;
			m_pPlayer[2]->property._transform._rotation = 180;
		}
		if (index->_name == "playerYellow"){
			m_pPlayer[3]->property._transform._translation = index->_transform._translation;
			m_pPlayer[3]->property._transform._rotation = 180;
		}
		if (index->_name == "boss"){
			m_pBoss->property._transform._translation = index->_transform._translation;
			m_pBoss->property._transform._rotation._y = 180;
		}
	}
	reader.UnLoad();

	for (int i = 0; i < m_pPlayer.size(); ++i){
		m_pPlayer[i]->property._transform._translation._y = 0;
	}
	m_pBoss->property._transform._translation._y = 0;


	m_view.property._rotation = Vector3(-3, 35, 0);
	m_view.property._translation = Vector3(-129, 43, -420);


	m_pSkybox = std::make_unique<Skybox>();
	m_pSkybox->Initialize();
	m_pSkybox->SetCamera(&m_view);
	m_pSkybox->SetTexture(ResourceManager::mGetInstance().GetTexture("skybox").get());

	return true;
}

void SceneEnd::Finalize(){

}

bool SceneEnd::Updater(){

	return true;
}

void SceneEnd::Render(){
	m_view.Render();
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	m_pSkybox->Render(shaderHash["texture"].get());
	m_field.mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	for (int i = 0; i < m_pPlayer.size(); i++){
		m_pPlayer[i]->KeyframeAnimationRender(shaderHash["texture"].get());
	}
	m_pBoss->KeyframeAnimationRender(shaderHash["texture"].get());

}

void SceneEnd::UIRender(){

}

bool SceneEnd::TransitionIn(){

	return true;
}

bool SceneEnd::TransitionOut(){

	return true;
}