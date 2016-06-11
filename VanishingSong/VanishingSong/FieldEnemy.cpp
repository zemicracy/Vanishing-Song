#include "FieldEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"
#include <Singleton.h>
#include "Debug.h"
#include <Sphere.h>
#include"GameManager.h"
using namespace aetherClass;

FieldEnemy::FieldEnemy(){
	m_message.clear();
}

FieldEnemy::~FieldEnemy()
{
	mFinalize();
}

bool FieldEnemy::mInitialize(eMusical type, ViewCamera* camera, std::string dataPath){
	m_dataPath = dataPath;
	mInitializeEnemy(type, camera);
	mInitializeEnemyColider(camera);
	
	return true;
}
bool FieldEnemy::mInitializeEnemy(eMusical type, aetherClass::ViewCamera* camera){
	m_property._penemy = ResourceManager::mGetInstance().mGetEnemyHash(type);
	m_property._penemy->property._transform._scale = 2;
	m_property._penemy->SetCamera(camera);
	return true;
}



//コライダーの初期化
void FieldEnemy::mInitializeEnemyColider(ViewCamera* camera){
	m_property._pCollider = std::make_shared<Cube>();
	m_property._pCollider->Initialize();
	m_property._pCollider->property._transform._translation = m_property._penemy->property._transform._translation;
	m_property._pCollider->property._transform._scale = Vector3(10, 10, 10);
	m_property._pCollider->property._color = Color(1, 1, 1, 0.5);
	m_property._pCollider->SetCamera(camera);

}

//更新処理
void FieldEnemy::mUpdate(){
	m_property._pCollider->property._transform._translation = m_property._penemy->property._transform._translation;
}

void FieldEnemy::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
	m_property._penemy->Render(model_shader);
	m_property._pCollider->Render(colider_shader);
}


FieldEnemy::Property& FieldEnemy::mGetProperty(){
	return m_property;
}

void FieldEnemy::mFaceToPlayer(aetherClass::Vector3 position){
//	m_charaEntity.mFaceToObject(m_pTopGear, position,"=");
}
void FieldEnemy::mFinalize(){

	if (m_property._pCollider){
		m_property._pCollider->Finalize();
		m_property._pCollider.reset();
		m_property._pCollider = nullptr;
	}

	if (m_property._penemy){
		m_property._penemy.reset();
	}
	m_pTopGear = nullptr;

	for (auto index : m_message){
		index.reset();
		index = nullptr;
	}
	m_message.clear();
}


//　登録用
void FieldEnemy::mRegisterMessage(std::string path){
	m_message.resize(m_message.size() + 1);
	const int id = m_message.size() - 1;
	m_message[id] = std::make_shared<Texture>();
	m_message[id]->Load(path);
}
int FieldEnemy::mGetMessageNum()const{
	return m_message.size();
}

std::shared_ptr<aetherClass::Texture> FieldEnemy::mGetMessage(const int id){
	return m_message[id];
}

std::string FieldEnemy::mGetBattleDataPath(){
	return m_dataPath;
}