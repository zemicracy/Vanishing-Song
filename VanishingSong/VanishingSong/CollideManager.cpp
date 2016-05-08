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
	m_enemyManager = enemy;
	m_filed = field;
	m_playerNumber = NULL;
}

//
bool key = false;
void CollideManager::mUpdate(){
	// �v���C���[�̂����Ԃ̊���o��
	const int playerNumber = mCheckPlayerFieldArea();
	mCheckHitObject(playerNumber);
	mCheckFieldAreaBullet();
	mCheckHitPlayerAttack(playerNumber);
	mCheckHitEnemyAttack(playerNumber);

	if (GameController::GetKey().KeyDownTrigger(VK_TAB)){
		key = !key;
		if (!key){
			m_player->mSetTarget(nullptr);
		}
		else{
			m_player->mSetTarget(m_enemyManager->mEnemyGet(m_playerNumber)[0]);
		}
	}
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

	// �v���C���[�ƓG
	for (auto enemy : m_enemyManager->mEnemyGet(number)){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *enemy->mGetProperty()._pcolider)){
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

// �v���C���[�̍U�����G�ɓ������Ă��邩�̊m�F
void CollideManager::mCheckHitPlayerAttack(const int playerNumber){
	// �e���������Ă��邩�̊m�F
	for (auto& bullet : m_player->mGetBullet()){
		if (!bullet._isRun)continue;
		for (auto& enemy : m_enemyManager->mEnemyGet(bullet._number)){
			if (CollideBoxOBB(*enemy->mGetProperty()._pcolider, *bullet._bullet->mGetCollider())){
				// �G�ƒe���������Ă�����
				bullet._isRun = false;
				enemy->mEnemyOnHit();

				// �G��|��������ǉ�
				m_player->mGetResultData()._killEnemy += 1;
			}
		}
	}

	if (m_player->mGetCommandType() != eCommandType::eShortDistanceAttack)return;
	// �ߐڍU�����������Ă��邩�̊m�F

}

// �G�̍U�����v���C���[�ɓ������Ă��邩�̊m�F
void CollideManager::mCheckHitEnemyAttack(const int playerNumber){
	// �����G���A���̓G�̂ݔ��������
	for (auto enemy : m_enemyManager->mEnemyGet(playerNumber)){
		// �����G���U����Ԃ���Ȃ��Ȃ牽�����Ȃ�
		//if (enemy->mGetCharaStatus()._action != eActionType::eAttack)continue;
		
		// �q�b�g�������_�ŏI��点��
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *enemy->mGetProperty()._pcolider)){
			m_player->mOnHitEnemyAttack(CharaStatus());
			break;
		}
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

		// ���ɂ��������Ă��Ȃ����������
		bullet._isRun = false;
		bullet._number = m_playerNumber;
	}
}