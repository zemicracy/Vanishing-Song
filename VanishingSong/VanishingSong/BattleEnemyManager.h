#ifndef _BATTLEENEMYMANAGER_H
#define _BATTLEENEMYMANAGER_H
#include "BattleEnemy.h"
#include"BattleField.h"
#include <ShaderBase.h>
#include "CharaStatus.h"
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
	void mInitialize(aetherClass::ViewCamera* camera,BattleField* );

	

	void mUpadate(const float timeScale);

	std::vector<eMusical> GetList();

	void ResetEnemyList(int,aetherClass::ViewCamera*camera);

	void misDie();

	/*
		•`‰æˆ—
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);

	/*
		“G‚ª‘S–Å‚Étrue‚ğ•Ô‚·‚æ‚¤‚É‚·‚é
	*/
	bool mIsEnd();

	int mGetWaveAllCount();
	CharaStatus& mGetCharaStatus(int);

private:
	void mFinalize();
	void mLoadInfo(std::string, BattleField*, aetherClass::ViewCamera*);
	int mGetRandom();
	eMusical mGetEnemyColor(const char);
	eEnemyType mGetEnemyType(const char);
	eMusical mGetEnemyAttack(const char);


private:
	
	BattleField* m_BattleField;

	aetherClass::ViewCamera* m_camera;
	bool flag;

	std::vector<std::shared_ptr<BattleEnemy>> m_pEnemy;
	std::vector<eMusical> m_enemyList;
	std::vector<std::vector<std::pair<eMusical, eEnemyType>>> m_waveEnemyList;
	std::vector<std::vector<eMusical>> m_enemyAttackList;



	std::vector<CharaStatus> m_hp;

	

	bool m_isEnd;
	int m_waveAllCount;
	int m_attackAllCount;
	

};

#endif