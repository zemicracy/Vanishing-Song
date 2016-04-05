#include "EnemyGround.h"
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
	
	m_pGearFrame = std::make_shared<GearFrame>();


	GetCharaStatus()._nowAction = eActionType::eWait;

	// 体のパーツ
	m_pGearFrame->m_pBody = GetCharaEntity().mSetUpGear("null", Gear::eType::eBody, camera);

	// 腰のパーツ
	m_pGearFrame->m_pWaist = GetCharaEntity().mSetUpGear("null", Gear::eType::eWaist, camera);

	// 体にパーツとの親子関係
	GetCharaEntity().mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
	
	m_pGearFrame->m_pBody->_pColider->property._transform._position._x += 0.01f;
	m_pGearFrame->m_pWaist->_pColider->property._transform._position._y -= 2.0f;

	return true;

}

void EnemyGround::mUpdate(){

	ChangeAction();

	switch (GetCharaStatus()._nowAction)
	{
	case eActionType::eWait:
		Debug::mPrint("Wait");
		break;
	case eActionType::eMove:
		Debug::mPrint("Move");
		break;
	case eActionType::eNull:
		Debug::mPrint("Null");
		break;
	case eActionType::eAttack:
		Debug::mPrint("Attack");
		break;
	case eActionType::eDie:
		EnemyDie();
		Debug::mPrint("Die");
		break;
	}

}

void EnemyGround::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	if (!m_pGearFrame->m_pBody)return;

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	GetCharaEntity().mGearRender(m_pGearFrame->m_pBody, model_shader, colider_shader);

}

void EnemyGround::EnemyDie(){

	m_pGearFrame->m_pBody.reset();
}


void EnemyGround::ChangeAction(){

	if (GameController::GetKey().IsKeyDown('R')){
		GetCharaStatus()._nowAction = eActionType::eDie;
	}
	
	
}