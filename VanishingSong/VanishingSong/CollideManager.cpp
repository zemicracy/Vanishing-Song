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
	// プレイヤーのいる空間の割り出し
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

// 障害物と当たったら止まる処理
void CollideManager::mCheckHitObject(const int number){

	// プレイヤーと壁
	for (auto wall : m_filed->mGetPartitionWall(number)){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *wall)){
			m_player->mOnHitWall();
			break;
		}
	}

	// プレイヤーと敵
	for (auto enemy : m_enemyManager->mEnemyGet(number)){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *enemy->mGetProperty()._pcolider)){
			m_player->mOnHitWall();
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
	for (int id = m_playerNumber; id < kPartitionSize; ++id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_playerNumber = id;
			return m_playerNumber;
		}
	}

	// 前回の番号からマイナスしていく
	for (int id = m_playerNumber; id >= 0; --id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_playerNumber = id;
			return m_playerNumber;
		}
	}

	return m_playerNumber;
}

// プレイヤーの攻撃が敵に当たっているかの確認
void CollideManager::mCheckHitPlayerAttack(const int playerNumber){
	// 弾が当たっているかの確認
	for (auto& bullet : m_player->mGetBullet()){
		if (!bullet._isRun)continue;
		for (auto& enemy : m_enemyManager->mEnemyGet(bullet._number)){
			if (CollideBoxOBB(*enemy->mGetProperty()._pcolider, *bullet._bullet->mGetCollider())){
				// 敵と弾が当たっていたら
				bullet._isRun = false;
				enemy->mEnemyOnHit();

				// 敵を倒した数を追加
				m_player->mGetResultData()._killEnemy += 1;
			}
		}
	}

	if (m_player->mGetCommandType() != eCommandType::eShortDistanceAttack)return;
	// 近接攻撃が当たっているかの確認

}

// 敵の攻撃がプレイヤーに当たっているかの確認
void CollideManager::mCheckHitEnemyAttack(const int playerNumber){
	// 同じエリア内の敵のみ判定をする
	for (auto enemy : m_enemyManager->mEnemyGet(playerNumber)){
		// もし敵が攻撃状態じゃないなら何もしない
		//if (enemy->mGetCharaStatus()._action != eActionType::eAttack)continue;
		
		// ヒットした時点で終わらせる
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

		// 何にもあたっていなかったら消す
		bullet._isRun = false;
		bullet._number = m_playerNumber;
	}
}