#include "EnemyDie.h"


EnemyDie::EnemyDie()
{
}


EnemyDie::~EnemyDie()
{
}

void EnemyDie::UpdateRun(EnemyBase::Property* enemy){
	enemy->m_isRender = true;
}