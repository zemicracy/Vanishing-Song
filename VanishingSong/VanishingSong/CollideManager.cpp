#include "CollideManager.h"
#include <Physics.h>
#include "Const.h"
#include <GameController.h>
using namespace aetherFunction;
using namespace aetherClass;
namespace{
	const int kWallCount = 2;
}
CollideManager::CollideManager(std::shared_ptr<FieldPlayer> player, std::shared_ptr<FieldArea> field, std::shared_ptr<FieldEnemyManager> enemy)
{ 
	m_player = player;
	m_filed = field;
	m_enemy = enemy;
	m_messageInfo.first = NULL;
	m_messageInfo.second = false;
}


CollideManager::~CollideManager()
{
}

//
void CollideManager::mUpdate(){
	// �v���C���[�̂����Ԃ̊���o��
	const int playerNumber = mCheckPlayerFieldArea();
	mCheckHitObject(playerNumber);
	mCheckHitEnemy(playerNumber);

	return;
}

// ��Q���Ɠ���������~�܂鏈��
void CollideManager::mCheckHitObject(const int number){

	// �v���C���[�ƕ�
	for (auto wall : m_filed->mGetPartitionWall(number)){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *wall)){
			m_player->mOnHitWall();
			break;
		}
	}

	return;
}

void CollideManager::mCheckHitEnemy(const int number){

	if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_enemy->mEnemyGet(number)->mGetProperty()._pCollider.get())){
		m_player->mOnHitWall();
		
	}

	const float x = m_player->mGetBodyColldier()->property._transform._translation._x - m_enemy->mEnemyGet(number)->mGetProperty()._pCollider->property._transform._translation._x;
	const float z = m_player->mGetBodyColldier()->property._transform._translation._z - m_enemy->mEnemyGet(number)->mGetProperty()._pCollider->property._transform._translation._z;
	if ((x*x) + (z*z) < 25 * 25){
		m_messageInfo.first = number;
		m_messageInfo.second = true;
	}else{
		m_messageInfo.second = false;
	}
}

std::pair<int,bool> CollideManager::GetMassageInfo(){

	return m_messageInfo;
}

/*
	���݂̃v���C���[�̋�Ԃ��擾
	�i�[�ꏊ m_fieldNumber
*/
int CollideManager::mCheckPlayerFieldArea(){
	
	// �O��̔ԍ�����v���X���Ă���
	for (int id = m_player->mGetFieldNumber(); id < kPartitionSize; ++id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_player->mSetFieldNumber(id);
			return id;
		}
	}

	// �O��̔ԍ�����}�C�i�X���Ă���
	for (int id = m_player->mGetFieldNumber(); id >= 0; --id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_player->mSetFieldNumber(id);
			return id;
		}
	}

	return m_player->mGetFieldNumber();
}