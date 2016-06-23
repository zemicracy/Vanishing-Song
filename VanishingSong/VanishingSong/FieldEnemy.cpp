#include "FieldEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"
#include <Singleton.h>
#include "Debug.h"
#include <Sphere.h>
#include"GameManager.h"
using namespace aetherClass;

FieldEnemy::FieldEnemy(){
	m_messagePath.clear();
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
	m_property._pEnemy = ResourceManager::mGetInstance().mGetEnemyHash(type);
	m_property._pEnemy->property._transform._scale = Vector3(-1,1,1);

	m_property._pEnemy->SetCamera(camera);
	return true;
}



//コライダーの初期化
void FieldEnemy::mInitializeEnemyColider(ViewCamera* camera){
	m_property._pCollider = std::make_shared<Cube>();
	m_property._pCollider->Initialize();
	m_property._pCollider->property._transform._translation = m_property._pEnemy->property._transform._translation;
	m_property._pCollider->property._transform._scale = Vector3(10, 10, 10);
	m_property._pCollider->property._color = Color(1, 1, 1, 0.5);
	m_property._pCollider->SetCamera(camera);

}

//更新処理
void FieldEnemy::mUpdate(){
	m_property._pCollider->property._transform._translation = m_property._pEnemy->property._transform._translation;
	m_property._pEnemy->KeyframeUpdate("wait", true);
}

void FieldEnemy::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
	m_property._pEnemy->KeyframeAnimationRender(model_shader);
	//m_property._pCollider->Render(colider_shader);
}


FieldEnemy::Property& FieldEnemy::mGetProperty(){
	return m_property;
}

void FieldEnemy::mFaceToPlayer(aetherClass::Vector3 position){
	m_charaEntity.mFaceToObject(m_property._pEnemy, position);
}
void FieldEnemy::mFinalize(){

	if (m_property._pCollider){
		m_property._pCollider->Finalize();
		m_property._pCollider.reset();
		m_property._pCollider = nullptr;
	}

	if (m_property._pEnemy){
		m_property._pEnemy.reset();
	}
	
}


//　登録用
void FieldEnemy::mRegisterMessage(std::string path){
	m_messagePath.push_back(path);
	m_messagePath.shrink_to_fit();
}
int FieldEnemy::mGetMessageNum()const{
	return m_messagePath.size();
}

std::string FieldEnemy::mGetMessage(const int id){
	return m_messagePath[id];
}

std::string FieldEnemy::mGetBattleDataPath(){
	return m_dataPath;
}

void FieldEnemy::mRegisterCannnotMessage(std::string path){
	m_cannotMessagePath = path;
}

std::string FieldEnemy::mGetCannotMessga(){
	return m_cannotMessagePath;
}

void FieldEnemy::mResetTransform(){
	m_property._pEnemy->property._transform = m_initTransform;
}

void FieldEnemy::mSetTransform(aetherClass::Transform tras){
	m_initTransform = tras;
}


std::shared_ptr<aetherClass::Texture>& FieldEnemy::mGetIcon(){
	return m_pIcon;
}


void FieldEnemy::mRegisterIcon(std::string path){
	if (m_pIcon){
		return;
	}

	m_pIcon = std::make_shared<Texture>();
	m_pIcon->Load(path);
	return;
}