#include "EnemyBase.h"

EnemyBase::EnemyBase()
{
}

CharaEntity EnemyBase::GetCharaEntity()
{
	return m_charaEntity;
}

CharaStatus& EnemyBase::GetCharaStatus(){
	return m_status;
}

EnemyBase::Property& EnemyBase::GetProperty(){
	return m_property;
}

