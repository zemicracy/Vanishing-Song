#include "FieldEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"
#include <Singleton.h>
#include "Debug.h"
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
	m_type = type;
	m_isTalking = false;
	m_animationCount = NULL;
	m_prevAnimationName = "null";
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
	m_property._pCollider = std::make_shared<Sphere>(10,10);
	m_property._pCollider->Initialize();
	m_property._pCollider->property._transform._translation = m_property._pEnemy->property._transform._translation;
	m_property._pCollider->property._transform._scale = Vector3(10, 10, 10);
	m_property._pCollider->property._color = Color(1, 1, 1, 0.5);
	m_property._pCollider->SetCamera(camera);

}

//更新処理
void FieldEnemy::mUpdate(std::string name){
	m_property._pCollider->property._transform._translation = m_property._pEnemy->property._transform._translation + Vector3(0,10,0);

	if (m_isTalking){
		if (m_prevAnimationName != "wait"){
			m_animationCount = NULL;
			m_prevAnimationName = "wait";
		}

		if (m_property._pEnemy->GetKeyframeCount("wait") - 1 < m_animationCount){
			m_animationCount = NULL;
		}

		m_property._pEnemy->KeyframeUpdate("wait", m_animationCount);
		m_animationCount += 1;
	}
	else{
		if (m_prevAnimationName != name){
			m_animationCount = NULL;
			m_prevAnimationName = name;
		}

		if (m_property._pEnemy->GetKeyframeCount(name)-1 < m_animationCount){
			m_animationCount = NULL;
		}
		m_property._pEnemy->KeyframeUpdate(name, m_animationCount);
		m_animationCount += 1;
	}
}

void FieldEnemy::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
	m_property._pEnemy->KeyframeAnimationRender(model_shader);
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

eMusical FieldEnemy::mGetType(){
	return m_type;
}

void FieldEnemy::mIsTalking(const bool talk){
	m_isTalking = talk;
}