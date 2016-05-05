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
		
	if (!enemy->_moveFlag){
		std::random_device randam;
		std::mt19937 mt(randam());

		std::uniform_int_distribution<> create(enemy->_enemyMoveRange.enemy_min_x, enemy->_enemyMoveRange.enemy_max_x);
		std::uniform_int_distribution<> create2(enemy->_enemyMoveRange.enemy_min_z, enemy->_enemyMoveRange.enemy_max_z);

		enemy->_enemyRandomMoveValue.randomValuex = create(randam);
		enemy->_enemyRandomMoveValue.randomValuez = create2(randam);

		enemy->_nextDirection = Vector3(enemy->_enemyRandomMoveValue.randomValuex, enemy->_penemy->m_pBody->_pGear->property._transform._translation._y, enemy->_enemyRandomMoveValue.randomValuez) - enemy->_penemy->m_pBody->_pGear->property._transform._translation;

		float rad = atan2(enemy->_nextDirection._x, enemy->_nextDirection._z);
		float rotationY = rad / kAetherPI * 180;

		//enemy->_penemy->m_pBody->_pGear->property._transform._rotation._y = rotationY;
		enemy->_moveFlag = true;
		Debug::mPrint(std::to_string(enemy->_moveFlag));
	}

	if (enemy->_moveFlag==true){
		mTargetMove(enemy);
	}

}

void EnemyMove::mTargetMove(EnemyBase::Property* enemy){

	//enemy->_penemy->m_pBody->_pGear->property._transform._translation += enemy->_nextDirection / 400;

	//enemy->_penemy->m_pBody->_pGear->property._transform._translation += enemy->_nextDirection.Normalize() * 2;
	
	m_charaEntity.mGearMove(enemy->_penemy->m_pBody, enemy->_nextDirection.Normalize() * 1.3);

}