#include "EnemyWait.h"


EnemyWait::EnemyWait()
{
}


EnemyWait::~EnemyWait()
{
}

void EnemyWait::mUpdateRun(EnemyBase::Property* enemy, aetherClass::Vector3* player_pos){

	enemy->_moveFlag = false;

}
	