#include "CollideManager.h"
#include <Physics.h>
#include "Const.h"
#include <GameController.h>
using namespace aetherFunction;
using namespace aetherClass;
namespace{
	const int kWallCount = 2;
	const int kBossNumber = 4;
	const float kRange = 40.f;
}
CollideManager::CollideManager(std::shared_ptr<FieldPlayer> player, std::shared_ptr<FieldArea> field, std::shared_ptr<FieldEnemyManager> enemy, std::shared_ptr<CageManager> cage)
{ 
	m_player = player;
	m_filed = field;
	m_enemy = enemy;
	m_cage = cage;
	m_messageInfo.first = NULL;
	m_messageInfo.second = false;
}


CollideManager::~CollideManager()
{
	m_enemy = nullptr;
	m_filed = nullptr;
	m_player = nullptr;
}

//
void CollideManager::mUpdate(){
	// プレイヤーのいる空間の割り出し
	const int playerNumber = mCheckPlayerFieldArea();
	mCheckHitObject(playerNumber);
	mCheckHitEnemy(playerNumber);
	mCheckHitCage(playerNumber);
	return;
}

// 障害物と当たったら止まる処理
void CollideManager::mCheckHitObject(const int number){

	// プレイヤーと家
	for (auto& objects : m_filed->mGetObjectList()){
		if (ColliderBoxSphere(*m_player->mGetSphereColldier(), *objects)){
			m_player->mOnHitObject(objects.get());
			break;
		}
	}

	// プレイヤーと壁
	for (auto& wall : m_filed->mGetWallList()){
		if (CollideBoxOBB(*m_player->mGetBodyColldier(), *wall)){
			m_player->mOnHitWall(wall.get());
			break;
		}
	}

	return;
}

//
void CollideManager::mCheckHitEnemy(const int number){

	const float x = m_player->mGetBodyColldier()->property._transform._translation._x - m_enemy->mEnemyGet(number)->mGetProperty()._pCollider->property._transform._translation._x;
	const float z = m_player->mGetBodyColldier()->property._transform._translation._z - m_enemy->mEnemyGet(number)->mGetProperty()._pCollider->property._transform._translation._z;
	if ((x*x) + (z*z) > kRange*kRange){
		m_messageInfo.first = number;
		m_messageInfo.second = true;
	}
	else{
		m_messageInfo.second = false;
	}

	if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_enemy->mEnemyGet(number)->mGetProperty()._pCollider.get())){
		m_player->mOnHitWall(m_enemy->mEnemyGet(number)->mGetProperty()._pCollider.get());
	}
}

//
void CollideManager::mCheckHitCage(const int number){
	if (number >= 3)return;
	const float x = m_player->mGetBodyColldier()->property._transform._translation._x - m_cage->mGetPosition(number)._x;
	const float z = m_player->mGetBodyColldier()->property._transform._translation._z - m_cage->mGetPosition(number)._z;

	if ((x*x) + (z*z) < kRange*kRange&&m_messageInfo.second){
		m_cage->mSetIsComment(number, true);
	}
	else{
		m_cage->mSetIsComment(number, false);
	}

	if (CollideBoxOBB(*m_player->mGetBodyColldier(), *m_cage->mGetColldier(number).get())){
		m_player->mOnHitWall(m_cage->mGetColldier(number).get());
	}
}

//
std::pair<int, bool>& CollideManager::GetMassageInfo(){
	return m_messageInfo;
}

/*
	現在のプレイヤーの空間を取得
	格納場所 m_fieldNumber
*/
int CollideManager::mCheckPlayerFieldArea(){
	
	// 前回の番号からプラスしていく
	for (int id = m_player->mGetFieldNumber(); id <5; ++id){
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