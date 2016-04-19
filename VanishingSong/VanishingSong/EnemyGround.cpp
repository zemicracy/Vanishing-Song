#include "EnemyGround.h"
#include "EnemyMove.h"
#include "EnemyDie.h"
#include "EnemyWait.h"
#include <WorldReader.h>
#include "Debug.h"

using namespace aetherClass;

EnemyGround::EnemyGround(){

}

EnemyGround::~EnemyGround()
{
	mFinalize();
}

bool EnemyGround::mSetUp(){
	return true;
}


bool EnemyGround::mInitialize(ViewCamera* camera){

	mInitializeEnemyColider(camera,m_pCollider);

	GetProperty().m_isRender = false;

	GetProperty()._enemy = std::make_shared<GearFrame>();

	GetCharaStatus()._nowAction = eActionType::eWait;

	// 体のパーツ
	GetProperty()._enemy->m_pBody = GetCharaEntity().mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eBody, camera);

	// 腰のパーツ
	GetProperty()._enemy->m_pWaist = GetCharaEntity().mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eWaist, camera);

	WorldReader read;
	read.Load("data\\Enemy.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			GetProperty()._enemy->m_pBody->_pGear->property._transform._translation._y += 20;
		}

		if (index->_name == "West"){
			GetProperty()._enemy->m_pWaist->_pGear->property._transform._translation._y += 10;
		}

	}
	read.UnLoad();

	// 体にパーツとの親子関係
	GetCharaEntity().mCreateRelationship(GetProperty()._enemy->m_pBody, GetProperty()._enemy->m_pWaist);

	return true;

}

void EnemyGround::mInitializeEnemyColider(aetherClass::ViewCamera* camera, std::shared_ptr<aetherClass::Cube>& enemycolider){

	enemycolider = std::make_shared<Cube>();
	enemycolider->Initialize();
	enemycolider->property._transform._translation = Vector3(10, 10, 10);
	enemycolider->property._transform._scale = 5;
	enemycolider->property._color = Color(1, 1, 1, 0.5);
	enemycolider->SetCamera(camera);
}


void EnemyGround::mUpdate(){

	m_AI=GetAI();
	m_AI->UpdateRun(&GetProperty(),&GetProperty()._enemy->m_pBody->_pGear->property._transform._translation);
}

void EnemyGround::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	if (!GetProperty()._enemy->m_pBody)return;
	if (GetProperty().m_isRender)return;

	m_pCollider->Render(colider_shader);

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	GetCharaEntity().mGearRender(GetProperty()._enemy->m_pBody, model_shader, colider_shader);

}

std::shared_ptr<EnemyAI> EnemyGround::GetAI(){

	switch (GetCharaStatus()._nowAction)
	{
	case eActionType::eWait:
		return std::make_shared<EnemyWait>();
	case eActionType::eMove:
		return std::make_shared<EnemyMove>();
	case eActionType::eNull:
		
	case eActionType::eAttack:
		
	case eActionType::eDie:
		return std::make_shared<EnemyDie>();
		
	default:
		break;
	}

}

void EnemyGround::mFinalize(){
	if (GetProperty()._enemy)
	{
		GetProperty()._enemy->Release();
		GetProperty()._enemy.reset();
		GetProperty()._enemy= nullptr;
	}

	if (m_pCollider){
		m_pCollider->Finalize();
		m_pCollider.reset();
		m_pCollider = nullptr;
	}
}

void EnemyGround::GetSetEnemyAction(){
	
}

