#include "CharaEntity.h"
#include "EnemyMove.h"
#include<random>
#include "Debug.h"
#include "MathUtility.h"

using namespace aetherClass;

EnemyMove::EnemyMove()
{
	
}


EnemyMove::~EnemyMove()
{
}

void EnemyMove::mUpdateRun(EnemyBase::Property* enemy, aetherClass::Vector3* player_pos){
		
	enemy->_penemy->m_pBody->_pGear->property._transform._translation += enemy->_nextDirection / 400;

	if (enemy->flag){
		return;
	}


	std::random_device randam;
	std::mt19937 mt(randam());

	std::uniform_int_distribution<> create(enemy->_enemyMoveRange.enemy_min_x, enemy->_enemyMoveRange.enemy_max_x);
	std::uniform_int_distribution<> create2(enemy->_enemyMoveRange.enemy_min_z, enemy->_enemyMoveRange.enemy_max_z);
	
		
	 if(enemy->_isMoveJudge ==true){
		randomValuex = create(randam);
		randomValuez = create2(randam);
		enemy->_isMoveJudge = false;
	}
	
	
	enemy->_nextDirection = Vector3(randomValuex, enemy->_penemy->m_pBody->_pGear->property._transform._translation._y, randomValuez) - enemy->_penemy->m_pBody->_pGear->property._transform._translation;
	//m_charaEntity.mGearMove(enemy->_penemy->m_pBody, enemy->_penemy->m_pBody->_pGear->property._transform._translation);

	float rad = atan2(enemy->_nextDirection._x, enemy->_nextDirection._z);
	float rotationY = rad / kAetherPI * 180;

	//enemy->_penemy->m_pBody->_pGear->property._transform._rotation._y = rotationY;
	Debug::mPrint(std::to_string(enemy->_nextDirection._x)+std::to_string(enemy->_enemyAreaNo));
	Debug::mPrint(std::to_string(enemy->_nextDirection._z)+std::to_string(enemy->_enemyAreaNo));


	enemy->flag = true;
	
}

void EnemyMove::mTargetMove(EnemyBase::Property* enemy,float randomValue_x,float randomValue_z){

	

}