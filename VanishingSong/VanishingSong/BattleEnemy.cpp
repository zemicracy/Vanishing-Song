#include "BattleEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"

using namespace aetherClass;
BattleEnemy::BattleEnemy()
{
}

BattleEnemy::~BattleEnemy()
{
}

void BattleEnemy::mInitialize(eMusical type, ViewCamera* camera,Vector3& pos){

	switch (type)
	{
	case eMusical::eBlue:
		mInitializeBlue(camera,pos);
		break;
	case eMusical::eGreen:
		mInitializeGreen(camera, pos);
		break;

	}
}

bool BattleEnemy::mInitializeBlue(ViewCamera* camera,Vector3& pos){

	m_enemy._gearFrame = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eBlue);

	m_enemy._gearFrame = gearframe;

	// 最上位に当たるパーツの設定
	m_pTopGear = m_enemy._gearFrame->m_pBody;

	m_charaEntity.SetCamera(m_pTopGear, camera);

	WorldReader read;
	read.Load("data\\EnemyAirBattle.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			//SetLoadModelValue(m_enemy._gearFrame->m_pBody, index);
			m_enemy._gearFrame->m_pBody->_pGear->property._transform = index->_transform;
			m_enemy._gearFrame->m_pBody->_pGear->property._transform._scale = 0.5;
			m_enemy._gearFrame->m_pBody->_pGear->property._transform._translation._y += 5;
		}

		if (index->_name == "waist"){
			//SetLoadModelValue(m_enemy._gearFrame->m_pWaist, index);
			m_enemy._gearFrame->m_pWaist->_pGear->property._transform = index->_transform;
			m_enemy._gearFrame->m_pWaist->_pGear->property._transform._scale = 0.5;
			m_enemy._gearFrame->m_pWaist->_pGear->property._transform._translation._y += 5;
	
		}
	}
	read.UnLoad();

	m_charaEntity.mGearMove(m_pTopGear, pos, "+=");
	

	return true;

}

bool BattleEnemy::mInitializeGreen(ViewCamera* camera, Vector3& pos){

	m_enemy._gearFrame = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eGreen);

	m_enemy._gearFrame = gearframe;

	// 最上位に当たるパーツの設定
	m_pTopGear = m_enemy._gearFrame->m_pBody;

	m_charaEntity.SetCamera(m_pTopGear, camera);

	WorldReader read;
	read.Load("data\\EnemyAirBattle.aether");
	for (auto index : read.GetInputWorldInfo()._object){

		if (index->_name == "body"){
			//SetLoadModelValue(m_enemy._gearFrame->m_pBody, index);
			m_enemy._gearFrame->m_pBody->_pGear->property._transform = index->_transform;
			m_enemy._gearFrame->m_pBody->_pGear->property._transform._scale = 0.5;
			m_enemy._gearFrame->m_pBody->_pGear->property._transform._translation._y += 5;
		}

		if (index->_name == "waist"){
			//SetLoadModelValue(m_enemy._gearFrame->m_pWaist, index);
			m_enemy._gearFrame->m_pWaist->_pGear->property._transform = index->_transform;
			m_enemy._gearFrame->m_pWaist->_pGear->property._transform._scale = 0.5;
			m_enemy._gearFrame->m_pWaist->_pGear->property._transform._translation._y += 5;

		}
	}
	read.UnLoad();

	m_charaEntity.mGearMove(m_pTopGear, pos, "+=");


	return true;

}


BattleEnemy::Enemy& BattleEnemy::mGetEnemy(){

	return m_enemy;

}

void BattleEnemy::mUpdate(const float timescale){
	
}


void BattleEnemy::mOnAttack(){

}

void BattleEnemy::mOnDamage(){

}

void BattleEnemy::mRender(std::shared_ptr<ShaderBase> tex){

	m_charaEntity.mGearRender(m_pTopGear, tex.get(), tex.get());
	
}

void BattleEnemy::SetLoadModelValue(std::shared_ptr<Gear>& gear, ObjectInfo* info){

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

//GearFrameは解放しない
void BattleEnemy::Finalize(){

}