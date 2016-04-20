#include "EnemyDie.h"


EnemyDie::EnemyDie()
{
}


EnemyDie::~EnemyDie()
{
}

void EnemyDie::UpdateRun(EnemyBase::Property* enemy, aetherClass::Vector3* player_pos){
	enemy->m_isRender = true;
}