#include "CollideManager.h"
#include <Physics.h>
#include "Const.h"
using namespace aetherFunction;
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
	// プレイヤーのいる空間の割り出し
	mCheckFieldArea();
}

void CollideManager::mCheckFieldArea(){
	bool isHit = false;
	for (int id = m_filedNumber; id < kPartitionSize; ++id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_filedNumber = id;
			isHit = true;
			Debug::mPrint("今のエリア番号：" + std::to_string(id));
			break;
		}
	}

	if (!isHit){
		m_filedNumber = 0;
		mCheckFieldArea();
	}
}

void CollideManager::mHitPlayerAttack(){

}


void CollideManager::mHitEnemyAttack(){

}