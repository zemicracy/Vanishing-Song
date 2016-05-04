#include "EnemyBase.h"

EnemyBase::EnemyBase()
{
}

CharaEntity EnemyBase::mGetCharaEntity()
{
	return m_charaEntity;
}

CharaStatus& EnemyBase::mGetCharaStatus(){
	return m_status;
}

EnemyBase::Property& EnemyBase::mGetProperty(){
	return m_property;
}

EnemyBase::EnemyStatus& EnemyBase::mGetEnemyStatus(){

	return m_enemystatus;
}

