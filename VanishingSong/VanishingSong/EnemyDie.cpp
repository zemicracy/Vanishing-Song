#include "EnemyDie.h"


EnemyDie::EnemyDie()
{
}


EnemyDie::~EnemyDie()
{
}

void EnemyDie::mUpdateRun(EnemyBase::Property* enemy, aetherClass::Vector3* player_pos){
	enemy->m_isRender = true;
}