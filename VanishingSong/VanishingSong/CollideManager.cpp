#include "CollideManager.h"
#include <Physics.h>
#include "Const.h"
#include <GameController.h>
using namespace aetherFunction;
using namespace aetherClass;
namespace{
	const int kWallCount = 2;
}
CollideManager::CollideManager()
{
}


CollideManager::~CollideManager()
{
}

//
void CollideManager::mInitialize(std::shared_ptr<Player> player, std::shared_ptr<EnemyManager> enemy, std::shared_ptr<FieldArea> field){
	m_player = player;
	m_enemys = enemy;
	m_filed = field;
	m_filedNumber = NULL;
}

//
void CollideManager::mUpdate(){
	// �v���C���[�̂����Ԃ̊���o��
	mCheckFieldArea();
	mCheckHitWall(m_filedNumber);
	mCheckFieldAreaBullet();
}


void CollideManager::mCheckHitWall(const int number){

	// �v���C���[�p
	for (auto wall : m_filed->mGetPartitionWall(number)){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *wall)){
			m_player->mOnHitWall();
		
			break;
		}
	}

	// �e�p
	// �ǂɂԂ������������
	for (auto& bullet : m_player->mGetBullet()){
		for (auto wall : m_filed->mGetPartitionWall(bullet._number))
			if (CollideBoxOBB(*wall, *bullet._bullet->mGetCollider())){
			Debug::mPrint("���ׂɂԂ�����");
			bullet._isRun = false;
			break;
		}
	}
	return;
}

/*
	���݂̃v���C���[�̋�Ԃ��擾
*/
void CollideManager::mCheckFieldArea(){
	if (GameController::GetKey().KeyDownTrigger('M')){
		Debug::mPrint(std::to_string(m_filedNumber));
	}

	// �O��̔ԍ�����v���X���Ă���
	for (int id = m_filedNumber; id < kPartitionSize; ++id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_filedNumber = id;
			return;
		}
	}

	// �O��̔ԍ�����}�C�i�X���Ă���
	for (int id = m_filedNumber; id >= 0; --id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_filedNumber = id;
			return;
		}
	}

	return;
}

// �v���C���[�̍U�����G�ɓ������Ă��邩�̊m�F
void CollideManager::mCheckHitPlayerAttack(){
	// �e���������Ă��邩�̊m�F
	for (auto& bullet : m_player->mGetBullet()){
		
	}

	if (m_player->mGetNowCommandType() != eCommandType::eShortDistanceAttack)return;
	// �ߐڍU�����������Ă��邩�̊m�F

}

// �G�̍U�����v���C���[�ɓ������Ă��邩�̊m�F
void CollideManager::mCheckHitEnemyAttack(){
	bool isHit = false;

	if (!isHit)return;

	// �v���C���[�̏�Ԃɍ��킹�ă_���[�W�ʂ̌v�Z
	switch (m_player->mGetNowCommandType())
	{
	case eCommandType::eShield:
		break;
	case eCommandType::eStrongShield:
		break;
	default:
		break;
	}
}


void CollideManager::mCheckFieldAreaBullet(){
	
	for (auto& bullet:m_player->mGetBullet()){
		if (!bullet._isRun)continue;
		bool isHit = false;
		// �O��̔ԍ�����v���X���Ă���
		for (int id = bullet._number; id < kPartitionSize; ++id){
			if (CollideBoxOBB(*bullet._bullet->mGetCollider(), *m_filed->mGetPartitionCube(id))){
				bullet._number = id;
				break;
			}
		}

		// ���̒e�ɍs��
		if (isHit)continue;

		// �O��̔ԍ�����}�C�i�X���Ă���
		for (int id = bullet._number; id >= 0; --id){
			if (CollideBoxOBB(*bullet._bullet->mGetCollider(), *m_filed->mGetPartitionCube(id))){
				bullet._number = id;
				return;
			}
		}
	}
}