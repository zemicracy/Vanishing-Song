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
	// プレイヤーのいる空間の割り出し
	const int playerNumber = mCheckPlayerFieldArea();
	mCheckHitObject(playerNumber);
	mCheckHitEnemy(playerNumber);

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
	現在のプレイヤーの空間を取得
	格納場所 m_fieldNumber
*/
int CollideManager::mCheckPlayerFieldArea(){
	
	// 前回の番号からプラスしていく
	for (int id = m_player->mGetFieldNumber(); id < kPartitionSize; ++id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_player->mSetFieldNumber(id);
			return id;
		}
	}

	// 前回の番号からマイナスしていく
	for (int id = m_player->mGetFieldNumber(); id >= 0; --id){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_filed->mGetPartitionCube(id))){
			m_player->mSetFieldNumber(id);
			return id;
		}
	}

	return m_player->mGetFieldNumber();
}