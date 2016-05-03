#include "CharaEntity.h"
#include "EnemyMove.h"
#include "Debug.h"

using namespace aetherClass;

EnemyMove::EnemyMove()
{
}


EnemyMove::~EnemyMove()
{
}

void EnemyMove::mUpdateRun(EnemyBase::Property* enemy, aetherClass::Vector3* player_pos){
	
	m_charaEntity.mGearMove(enemy->_penemy->m_pBody, enemy->_penemy->m_pBody->_pGear->property._transform._translation,std::string("+"));

	//enemy->_penemy->m_pBody->_pGear->property._transform._translation._x += 1.0f;
	
	
}