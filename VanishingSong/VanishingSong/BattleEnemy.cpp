#include "BattleEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"

using namespace aetherClass;

namespace {
	const std::string kDefaultAnimationName = "wait";
	const int kMaxAttackFrame = 30;
}
BattleEnemy::BattleEnemy()
{
}

BattleEnemy::~BattleEnemy()
{
	Finalize();
}

void BattleEnemy::mInitialize(eMusical type,eEnemyType enemytype, ViewCamera* camera,Vector3& pos){

	m_isDie = false;
	
	m_enemy._model= std::make_shared<FbxModel>();

	m_enemy._model = ResourceManager::mGetInstance().mGetEnemyHash(type);
	m_enemy._model->SetCamera(camera);

	m_enemy._model->property._transform._translation = Vector3(pos._x,0,pos._z);

	m_enemy._model->property._transform._rotation._y = -90;

	if (type == eMusical::eAdlib){
		m_enemy._model->property._transform._scale -= 0.1;
	}

	m_enemy._model->SetModelColor(Color(0, 0, 0, 1));
	m_animationName = kDefaultAnimationName;
	m_type = type;
}

BattleEnemy::Enemy& BattleEnemy::mGetEnemy(){
	return m_enemy;
}

void BattleEnemy::mUpdate(const float timescale){
	//アニメーション false は 1Shot
	if (m_animationName != kDefaultAnimationName){
		m_enemy._model->KeyframeUpdate(m_animationName, m_animationFrame);
	}
	else{
		m_enemy._model->KeyframeUpdate(kDefaultAnimationName, true);
	}

	if (m_animationFrame >= kMaxAttackFrame - 1){
		m_animationName = kDefaultAnimationName;
	}

	++m_animationFrame;

	if (m_isDie){
		m_alpha -= 0.05;
		m_enemy._model->SetModelColor(Color(0, 0, 0, m_alpha));
	}
	else{
		m_enemy._model->SetModelColor(Color(0, 0, 0, 1));
	}
	
}


void BattleEnemy::mOnAttack(){

}

void BattleEnemy::mOnDamage(){

}

void BattleEnemy::mRender(std::shared_ptr<ShaderBase> tex){
	m_enemy._model->KeyframeAnimationRender(tex.get());
//	m_enemy._model->Render(tex.get());
}

void BattleEnemy::misDie(){
	m_isDie = true;
	m_alpha = 1;
}

void BattleEnemy::Finalize(){
	m_enemy._model->SetModelColor(Color(0, 0, 0, 1));
	if (m_enemy._model){
		m_enemy._model.reset();
	}
}

void BattleEnemy::mChangeAnimation(std::string name){
	m_animationFrame = 0;
	m_animationName = name;
}

eMusical BattleEnemy::mGetType(){
	return m_type;
}