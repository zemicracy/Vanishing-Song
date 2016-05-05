#ifndef _COLLIDEMANAGER_H
#define _COLLIDEMANAGER_H
#include "Player.h"
#include "EnemyManager.h"
#include "FieldArea.h"
class CollideManager
{
public:
	CollideManager();
	~CollideManager();
	void mInitialize(std::shared_ptr<Player> player, std::shared_ptr<EnemyManager> enemy, std::shared_ptr<FieldArea> field);
	void mUpdate();

private:
	int mCheckPlayerFieldArea();
	void mCheckFieldAreaBullet();
	void mCheckHitWall(const int number);
	void mCheckHitPlayerAttack(const int);
	void mCheckHitEnemyAttack(const int);
private:
	std::shared_ptr<Player> m_player;
	std::shared_ptr<EnemyManager> m_enemyManager;
	std::shared_ptr<FieldArea> m_filed;
	
	int m_filedNumber;
};

#endif