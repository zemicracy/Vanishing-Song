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

	m_enemy._gearFrame->m_pBody->_pGear->property._transform._scale = 2;

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

	m_enemy._gearFrame->m_pBody->_pGear->property._transform._scale = 2;

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

//GearFrameは解放しない
void BattleEnemy::Finalize(){

}