#include "EnemyGround.h"
#include <WorldReader.h>
#include "Debug.h"

using namespace aetherClass;

EnemyGround::EnemyGround()
{
}


EnemyGround::~EnemyGround()
{
}

bool EnemyGround::mSetUp(){
	return true;
}


bool EnemyGround::mInitialize(ViewCamera* camera){
	
	m_render = false;

	m_pGearFrame = std::make_shared<GearFrame>();

	GetCharaStatus()._nowAction = eActionType::eWait;

	// 体のパーツ
	m_pGearFrame->m_pBody = GetCharaEntity().mSetUpGear("null", Gear::eType::eBody, camera);

	// 腰のパーツ
	m_pGearFrame->m_pWaist = GetCharaEntity().mSetUpGear("null", Gear::eType::eWaist, camera);

	

	WorldReader read;
	read.Load("data\\Enemy.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			m_pGearFrame->m_pBody->_pColider->property._transform = index->_transform;
			
		}

		if (index->_name == "West"){
			m_pGearFrame->m_pWaist->_pColider->property._transform = index->_transform;
		}

	}
	read.UnLoad();


	// 体にパーツとの親子関係
	GetCharaEntity().mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
	
	m_pGearFrame->m_pBody->_pColider->property._transform._translation._x  += 0.01f;
	m_pGearFrame->m_pWaist->_pColider->property._transform._translation._y -= 2.0f;

	return true;

}

void EnemyGround::mUpdate(){

	mChangeAction();

	m_pGearFrame->m_pBody->_pColider->property._color = Color(1, 1, 1, 1);

	m_pGearFrame->m_pWaist->_pColider->property._color = Color(1, 1, 1, 1);

	switch (GetCharaStatus()._nowAction)
	{
	case eActionType::eWait:
		Debug::mPrint("Wait");
		break;
	case eActionType::eMove:
		Debug::mPrint("Move");
		mEnemyMove();
		break;
	case eActionType::eNull:
		Debug::mPrint("Null");
		break;
	case eActionType::eAttack:
		Debug::mPrint("Attack");
		break;
	case eActionType::eDie:
		mEnemyDie();
		Debug::mPrint("Die");
		break;
	}

}

void EnemyGround::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	if (!m_pGearFrame->m_pBody)return;
	if (m_render)return;

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	GetCharaEntity().mGearRender(m_pGearFrame->m_pBody, model_shader, colider_shader);

}

void EnemyGround::mEnemyDie(){

	m_render = true;

}


void EnemyGround::mChangeAction(){

	if (GameController::GetKey().IsKeyDown('R')){
		GetCharaStatus()._nowAction = eActionType::eDie;
	}

	if (GameController::GetKey().IsKeyDown('Q')){
		GetCharaStatus()._nowAction = eActionType::eMove;
	}
}

void EnemyGround::mEnemyMove(){
	
	m_pGearFrame->m_pBody->_pColider->property._transform._translation._x += 0.01f;
}