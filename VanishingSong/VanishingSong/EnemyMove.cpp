#include "EnemyMove.h"
#include "Debug.h"

EnemyMove::EnemyMove()
{
}


EnemyMove::~EnemyMove()
{
}

void EnemyMove::UpdateRun(EnemyBase::Property& enemy){
	
	enemy._transform._translation._x += 1.0;

}