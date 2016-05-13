#include "CollideManager.h"
#include <Physics.h>
#include "Const.h"
#include <GameController.h>
using namespace aetherFunction;
using namespace aetherClass;
namespace{
	const int kWallCount = 2;
}
CollideManager::CollideManager(std::shared_ptr<FieldPlayer> player, std::shared_ptr<FieldArea> field)
{ 
	m_player = player;
	m_filed = field;
	m_playerNumber = NULL;
}


CollideManager::~CollideManager()
{
}

//
void CollideManager::mUpdate(){
	// �v���C���[�̂����Ԃ̊���o��
	const int playerNumber = mCheckPlayerFieldArea();
	mCheckHitObject(playerNumber);

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

/*
	���݂̃v���C���[�̋�Ԃ��擾
	�i�[�ꏊ m_fieldNumber
*/
int CollideManager::mCheckPlayerFieldArea(){
	
	// �O��̔ԍ�����v���X���Ă���
	for (int id = m_playerNumber; id < kPartitionSize; ++id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_playerNumber = id;
			return m_playerNumber;
		}
	}

	// �O��̔ԍ�����}�C�i�X���Ă���
	for (int id = m_playerNumber; id >= 0; --id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_playerNumber = id;
			return m_playerNumber;
		}
	}

	return m_playerNumber;
}