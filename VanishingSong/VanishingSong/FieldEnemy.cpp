#include "FieldEnemy.h"
#include <WorldReader.h>
#include "Debug.h"

using namespace aetherClass;

FieldEnemy::FieldEnemy(){

}

FieldEnemy::~FieldEnemy()
{
	
}

//Enemy地上
bool FieldEnemy::mInitializeGround(ViewCamera* camera){

		m_property._penemy = std::make_shared<GearFrame>();

		// 体のパーツ
		m_property._penemy->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Ground\\Ground_Body.fbx", Gear::eType::eBody, camera, "Model\\Enemy\\Ground\\tex");
		// 腰のパーツ
		m_property._penemy->m_pWaist = m_charaEntity.mSetUpGear("Model\\Enemy\\Ground\\Ground_Head.fbx", Gear::eType::eWaist, camera, "Model\\Enemy\\Ground\\tex");

		WorldReader read;
		read.Load("data\\Enemy.aether");
		for (auto index : read.GetInputWorldInfo()._object){

			if (index->_name == "body"){
				SetLoadModelValue(m_property._penemy->m_pBody, index);
				m_property._penemy->m_pBody->_pGear->property._transform._scale = 4;
			}

			if (index->_name == "West"){
				SetLoadModelValue(m_property._penemy->m_pWaist, index);
				m_property._penemy->m_pWaist->_pGear->property._transform._scale = 4;
			}
		}
		read.UnLoad();

		// 最上位に当たるパーツの設定
		m_pTopGear = m_property._penemy->m_pBody;

		// 体にパーツとの親子関係
		m_charaEntity.mCreateRelationship(m_pTopGear, m_property._penemy->m_pWaist);
		return true;

}

//Enemy空中
bool FieldEnemy::mInitializeAir(ViewCamera* camera){

	m_property._penemy = std::make_shared<GearFrame>();

	// 体のパーツ
	m_property._penemy->m_pBody = m_charaEntity.mSetUpGear("Model\\Enemy\\Air\\Air_Body.fbx", Gear::eType::eBody, camera, "Model\\Enemy\\Air\\tex");
	// 腰のパーツ
	m_property._penemy->m_pWaist = m_charaEntity.mSetUpGear("Model\\Enemy\\Air\\Air_Head.fbx", Gear::eType::eWaist, camera, "Model\\Enemy\\Air\\tex");

	WorldReader read;
	read.Load("data\\Enemy.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			SetLoadModelValue(m_property._penemy->m_pBody, index);
			m_property._penemy->m_pBody->_pGear->property._transform._scale = 4;
		}

		if (index->_name == "West"){
			SetLoadModelValue(m_property._penemy->m_pWaist, index);
			m_property._penemy->m_pWaist->_pGear->property._transform._scale = 4;
		}
	}
	read.UnLoad();

	// 最上位に当たるパーツの設定
	m_pTopGear = m_property._penemy->m_pBody;

	// 体にパーツとの親子関係
	m_charaEntity.mCreateRelationship(m_pTopGear, m_property._penemy->m_pWaist);
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
	
	m_property._pCollider->Render(colider_shader);

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_pTopGear, model_shader, colider_shader);
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



