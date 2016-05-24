#include "FieldEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"
#include <Singleton.h>
#include "Debug.h"

using namespace aetherClass;

FieldEnemy::FieldEnemy(){

}

FieldEnemy::~FieldEnemy()
{
	
}

bool FieldEnemy::mInitialize(eType type,ViewCamera* camera){

	switch (type)
	{
	case eType::Ground:
		mInitializeGround(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Air:
		mInitializeAir(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Blue:
		mInitializeBlue(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Yellow:
		mInitializeYellow(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Null:

	default:
		break;
	}

	return true;
}

//Enemy地上
bool FieldEnemy::mInitializeGround(ViewCamera* camera){

		m_property._penemy = std::make_shared<GearFrame>();
	
		auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eRed);

		m_property._penemy = gearframe;

		WorldReader read;
		read.Load("data\\Enemy.aether");
		for (auto index : read.GetInputWorldInfo()._object){

			if (index->_name == "body"){
				SetLoadModelValue(m_property._penemy->m_pBody, index);
				m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;
			}

			if (index->_name == "waist"){
				//SetLoadModelValue(m_property._penemy->m_pWaist, index);
				m_property._penemy->m_pWaist->_pGear->property._transform._scale = 2;
			}
		}
		read.UnLoad();

		// 最上位に当たるパーツの設定
		m_pTopGear = m_property._penemy->m_pBody;

		// 体にパーツとの親子関係
		m_charaEntity.mCreateRelationship(m_pTopGear, m_property._penemy->m_pWaist);
		m_charaEntity.SetCamera(m_pTopGear,camera);

		return true;
}

//Enemy空中
bool FieldEnemy::mInitializeAir(ViewCamera* camera){

	m_property._penemy = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eGreen);
	m_property._penemy = gearframe;
	WorldReader read;
	read.Load("data\\EnemyAir.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			SetLoadModelValue(m_property._penemy->m_pBody, index);
			m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;
		}

		if (index->_name == "waist"){
			//SetLoadModelValue(m_property._penemy->m_pWaist, index);
			m_property._penemy->m_pWaist->_pGear->property._transform._scale = 2;
		}
	}
	read.UnLoad();

	// 最上位に当たるパーツの設定
	m_pTopGear = m_property._penemy->m_pBody;

	// 体にパーツとの親子関係
	m_charaEntity.mCreateRelationship(m_pTopGear, m_property._penemy->m_pWaist);
	m_charaEntity.SetCamera(m_pTopGear, camera);

	return true;

}

//仮用敵
bool FieldEnemy::mInitializeBlue(ViewCamera* camera){

	m_property._penemy = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eBlue);

	m_property._penemy = gearframe;

	WorldReader read;
	read.Load("data\\Enemy.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			SetLoadModelValue(m_property._penemy->m_pBody, index);
			m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;
		}

		if (index->_name == "waist"){
			//SetLoadModelValue(m_property._penemy->m_pWaist, index);
			m_property._penemy->m_pWaist->_pGear->property._transform._scale = 2;
		}
	}
	read.UnLoad();

	// 最上位に当たるパーツの設定
	m_pTopGear = m_property._penemy->m_pBody;

	// 体にパーツとの親子関係
	m_charaEntity.mCreateRelationship(m_pTopGear, m_property._penemy->m_pWaist);
	m_charaEntity.SetCamera(m_pTopGear, camera);

	return true;
}

//仮用敵
bool FieldEnemy::mInitializeYellow(ViewCamera* camera){

	m_property._penemy = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eYellow);

	m_property._penemy = gearframe;

	WorldReader read;
	read.Load("data\\Enemy.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			SetLoadModelValue(m_property._penemy->m_pBody, index);
			m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;
		}

		if (index->_name == "waist"){
			//SetLoadModelValue(m_property._penemy->m_pWaist, index);
			m_property._penemy->m_pWaist->_pGear->property._transform._scale = 2;
		}
	}
	read.UnLoad();

	// 最上位に当たるパーツの設定
	m_pTopGear = m_property._penemy->m_pBody;

	// 体にパーツとの親子関係
	m_charaEntity.mCreateRelationship(m_pTopGear, m_property._penemy->m_pWaist);
	m_charaEntity.SetCamera(m_pTopGear, camera);

	return true;
}


//コライダーの初期化
void FieldEnemy::mInitializeEnemyColider(ViewCamera* camera){
	m_property._pCollider = std::make_shared<Cube>();
	m_property._pCollider->Initialize();
	m_property._pCollider->property._transform._translation = m_pTopGear->_pGear->property._transform._translation;
	m_property._pCollider->property._transform._scale = Vector3(9, 12, 4);
	m_property._pCollider->property._color = Color(1, 1, 1, 0.5);
	m_property._pCollider->SetCamera(camera);

}

//更新処理
void FieldEnemy::mUpdate(){
	m_property._pCollider->property._transform._translation = m_property._penemy->m_pBody->_pGear->property._transform._translation;
}

void FieldEnemy::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_pTopGear, model_shader, colider_shader);

	m_property._pCollider->Render(colider_shader);
}

void FieldEnemy::SetLoadModelValue(std::shared_ptr<Gear>& gear, ObjectInfo* info){

	gear->_pGear->property._transform = info->_transform;
	gear->_initialTransform = info->_transform;
	if (gear->_pParent)
	{
		std::shared_ptr<Gear> pParent = gear->_pParent;
		// 最上位との差
		gear->_topDifference._translation = gear->_pGear->property._transform._translation - m_pTopGear->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - m_pTopGear->_pGear->property._transform._rotation;

		// 親との差
		gear->_parentDifference._translation = gear->_pGear->property._transform._translation - pParent->_pGear->property._transform._translation;
		gear->_parentDifference._rotation = gear->_pGear->property._transform._rotation - pParent->_pGear->property._transform._rotation;
	}
}

FieldEnemy::Property& FieldEnemy::mGetProperty(){
	return m_property;
}

void FieldEnemy::mEnemyOnHit(){
	
}
void FieldEnemy::mFaceToPlayer(aetherClass::Vector3 position){
	m_charaEntity.mFaceToObject(m_pTopGear, position,"=");
}
void FieldEnemy::mFinalize(){

	if (m_property._penemy)
	{
		m_property._penemy->Release();
		m_property._penemy.reset();
		m_property._penemy = nullptr;
	}

	if (m_property._pCollider){
		m_property._pCollider->Finalize();
		m_property._pCollider.reset();
		m_property._pCollider = nullptr;
	}
	if (m_pTopGear)
	{
		m_pTopGear->Release();
		m_pTopGear.reset();
		m_pTopGear = nullptr;
	}

}



