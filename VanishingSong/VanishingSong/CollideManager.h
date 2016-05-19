#ifndef _COLLIDEMANAGER_H
#define _COLLIDEMANAGER_H
#include "FieldPlayer.h"
#include "FieldEnemyManager.h"
#include "FieldArea.h"
class CollideManager
{
public:
	CollideManager(std::shared_ptr<FieldPlayer> player, std::shared_ptr<FieldArea> field, std::shared_ptr<FieldEnemyManager> enemy);
	~CollideManager();
	void mUpdate();
	bool GetMassageFlag();

private:
	int mCheckPlayerFieldArea();
	void mCheckHitObject(const int number);
	void mCheckHitEnemy(const int number);

private:
	std::shared_ptr<FieldPlayer> m_player;
	std::shared_ptr<FieldArea> m_filed;
	std::shared_ptr<FieldEnemyManager> m_enemy;
	bool m_messageFlag;
	
};

#endif