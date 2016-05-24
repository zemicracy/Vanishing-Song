#ifndef _BATTLEENEMYMANAGER_H
#define _BATTLEENEMYMANAGER_H
#include "BattleEnemy.h"
#include"BattleField.h"
#include <ShaderBase.h>
#include <array>
namespace{
	const int kEnemyMaxNum = 4;
}
class BattleEnemyManager
{
public:
	BattleEnemyManager();
	~BattleEnemyManager();
	/*
		‰Šú‰»‚Ìˆø”‚Å‚Ç‚Ì“G‚ğ”z’u‚·‚é‚©‚ğæ“¾‚·‚é
		_type‚ªNull‚Ìê‡‚Í‰Šú‰»‚Ìˆ—‚ğ”ò‚Î‚·
	*/
	void Initialize(aetherClass::ViewCamera* camera,BattleField* );

	void mUpadate(const float timeScale);

	/*
		•`‰æˆ—
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);

	/*
		“G‚ª‘S–Å‚Étrue‚ğ•Ô‚·‚æ‚¤‚É‚·‚é
	*/
	bool mIsEnd();

private:
	void EnemySet(eMusical type, std::shared_ptr<GearFrame> gearFrame);
	bool m_isEnd;
	std::vector<std::shared_ptr<BattleEnemy>> m_pEnemy;
	BattleField* m_BattleField;

};

#endif