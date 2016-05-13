#ifndef _COLLIDEMANAGER_H
#define _COLLIDEMANAGER_H
#include "FieldPlayer.h"
#include "EnemyManager.h"
#include "FieldArea.h"
class CollideManager
{
public:
	CollideManager(std::shared_ptr<FieldPlayer> player, std::shared_ptr<FieldArea> field);
	~CollideManager();
	void mUpdate();

private:
	int mCheckPlayerFieldArea();
	void mCheckHitObject(const int number);
private:
	std::shared_ptr<FieldPlayer> m_player;
	std::shared_ptr<FieldArea> m_filed;
	
	int m_playerNumber;
};

#endif