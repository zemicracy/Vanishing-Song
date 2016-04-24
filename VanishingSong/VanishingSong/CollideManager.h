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
	void mCheckFieldArea();
	void mHitPlayerAttack();
	void mHitEnemyAttack();
private:
	std::shared_ptr<Player> m_player;
	std::shared_ptr<EnemyManager> m_enemys;
	std::shared_ptr<FieldArea> m_filed;

	int m_filedNumber;
};

#endif