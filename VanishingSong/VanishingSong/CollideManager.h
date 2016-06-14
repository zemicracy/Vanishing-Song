#ifndef _COLLIDEMANAGER_H
#define _COLLIDEMANAGER_H
#include "FieldPlayer.h"
#include "FieldEnemyManager.h"
#include "FieldArea.h"
#include "CageManager.h"
class CollideManager
{
public:
	CollideManager(std::shared_ptr<FieldPlayer> player, std::shared_ptr<FieldArea> field, std::shared_ptr<FieldEnemyManager> enemy, std::shared_ptr<CageManager> cageManager);
	~CollideManager();
	void mUpdate();
	std::pair<int,bool> GetMassageInfo();

private:
	int mCheckPlayerFieldArea();
	void mCheckHitObject(const int number);
	void mCheckHitEnemy(const int number);
	void mCheckHitCage(const int number);

private:
	std::shared_ptr<FieldPlayer> m_player;
	std::shared_ptr<FieldArea> m_filed;
	std::shared_ptr<FieldEnemyManager> m_enemy;
	std::shared_ptr<CageManager> m_cage;
	std::pair<int, bool> m_messageInfo;
	
};

#endif