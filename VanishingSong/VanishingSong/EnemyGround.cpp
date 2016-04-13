#include "EnemyGround.h"
#include "EnemyMove.h"
#include "EnemyDie.h"
#include "EnemyWait.h"
#include <WorldReader.h>
#include "Debug.h"

using namespace aetherClass;

EnemyGround::EnemyGround()
{
}


EnemyGround::~EnemyGround()
{
	mFinalize();
}

bool EnemyGround::mSetUp(){
	return true;
}


bool EnemyGround::mInitialize(ViewCamera* camera){
	
	GetProperty().m_isRender = false;

	GetProperty()._enemy = std::make_shared<GearFrame>();

	GetCharaStatus()._nowAction = eActionType::eWait;

	// �̂̃p�[�c
	GetProperty()._enemy->m_pBody = GetCharaEntity().mSetUpGear("null", Gear::eType::eBody, camera);

	// ���̃p�[�c
	GetProperty()._enemy->m_pWaist = GetCharaEntity().mSetUpGear("null", Gear::eType::eWaist, camera);

	WorldReader read;
	read.Load("data\\Enemy.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
		//	GetProperty()._enemy->m_pBody->_pColider->property._transform = index->_transform;
			
		}

		if (index->_name == "West"){
		//	GetProperty()._enemy->m_pWaist->_pColider->property._transform = index->_transform;
		}

	}
	read.UnLoad();


	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	GetCharaEntity().mCreateRelationship(GetProperty()._enemy->m_pBody, GetProperty()._enemy->m_pWaist);

	return true;

}

void EnemyGround::mUpdate(){

	mChangeAction();
	m_AI=GetAI();
	m_AI->UpdateRun(&GetProperty());
}

void EnemyGround::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	if (!GetProperty()._enemy->m_pBody)return;
	if (GetProperty().m_isRender)return;

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	GetCharaEntity().mGearRender(GetProperty()._enemy->m_pBody, model_shader, colider_shader);

}

void EnemyGround::mChangeAction(){

	if (GameController::GetKey().IsKeyDown('R')){
		GetCharaStatus()._nowAction = eActionType::eDie;
	}

	if (GameController::GetKey().IsKeyDown('Q')){
		GetCharaStatus()._nowAction = eActionType::eMove;
	}

	if (GameController::GetKey().IsKeyDown('W')){
		GetCharaStatus()._nowAction = eActionType::eWait;
	}

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
}