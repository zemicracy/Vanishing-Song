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
	m_filedNumber = NULL;
}

//
void CollideManager::mUpdate(){
	// プレイヤーのいる空間の割り出し
	const int playerNumber = mCheckPlayerFieldArea();
	mCheckHitWall(playerNumber);
	mCheckFieldAreaBullet();
	mCheckHitPlayerAttack(playerNumber);
	mCheckHitEnemyAttack(playerNumber);
}


void CollideManager::mCheckHitWall(const int number){

	// プレイヤー用
	for (auto wall : m_filed->mGetPartitionWall(number)){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *wall)){
			m_player->mOnHitWall();
		
			break;
		}
	}

	// 弾用
	// 壁にぶつかったら消える
	for (auto& bullet : m_player->mGetBullet()){
		if (!bullet._isRun)continue;
		for (auto wall : m_filed->mGetPartitionWall(bullet._number))
			if (CollideBoxOBB(*wall, *bullet._bullet->mGetCollider())){
			bullet._isRun = false;
			break;
		}
	}
	return;
}

/*
	現在のプレイヤーの空間を取得
	格納場所 m_fieldNumber
*/
int CollideManager::mCheckPlayerFieldArea(){
	
	// 前回の番号からプラスしていく
	for (int id = m_filedNumber; id < kPartitionSize; ++id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_filedNumber = id;
			return m_filedNumber;
		}
	}

	// 前回の番号からマイナスしていく
	for (int id = m_filedNumber; id >= 0; --id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_filedNumber = id;
			return m_filedNumber;
		}
	}

	return m_filedNumber;
}

// プレイヤーの攻撃が敵に当たっているかの確認
void CollideManager::mCheckHitPlayerAttack(const int playerNumber){
	// 弾が当たっているかの確認
	for (auto& bullet : m_player->mGetBullet()){
	
	}

	if (m_player->mGetCommandType() != eCommandType::eShortDistanceAttack)return;
	// 近接攻撃が当たっているかの確認

}

// 敵の攻撃がプレイヤーに当たっているかの確認
void CollideManager::mCheckHitEnemyAttack(const int playerNumber){
	bool isHit = false;
	// TODO:判定しようね
	if (!isHit)return;

	// プレイヤーの状態に合わせてダメージ量の計算
	switch (m_player->mGetCommandType())
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
		// 前回の番号からプラスしていく
		for (int id = bullet._number; id < kPartitionSize; ++id){
			if (CollideBoxOBB(*bullet._bullet->mGetCollider(), *m_filed->mGetPartitionCube(id))){
				bullet._number = id;
				break;
			}
		}

		// 次の弾に行く
		if (isHit)continue;

		// 前回の番号からマイナスしていく
		for (int id = bullet._number; id >= 0; --id){
			if (CollideBoxOBB(*bullet._bullet->mGetCollider(), *m_filed->mGetPartitionCube(id))){
				bullet._number = id;
				return;
			}
		}
	}
}