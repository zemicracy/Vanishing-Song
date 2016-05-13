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
	// プレイヤーのいる空間の割り出し
	const int playerNumber = mCheckPlayerFieldArea();
	mCheckHitObject(playerNumber);

	return;
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