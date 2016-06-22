#ifndef _BATTLEENEMYMANAGER_H
#define _BATTLEENEMYMANAGER_H
#include "BattleEnemy.h"
#include "BattleBoss.h"
#include"BattleField.h"
#include"Const.h"
#include <GameController.h>
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

	

	void mUpdate(const float timeScale);

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
	int mGetAppendOption();
	void mChangeAnimation(eBattleActionType,eMusical);

private:
	void mFinalize();
	void mLoadInfo(std::string, BattleField*, aetherClass::ViewCamera*);
	int mGetRandom();
	eMusical mGetEnemyColor(const char);
	eEnemyType mGetEnemyType(const char);
	eMusical mGetEnemyAttack(const char);
	
	GameManager::eFieldState m_fieldState;


private:
	
	BattleField* m_BattleField;

	aetherClass::ViewCamera* m_camera;
	bool flag;

	std::shared_ptr<BattleBoss> m_pBattle;
	std::unordered_map<eMusical,std::shared_ptr<BattleEnemy>>m_pEnemy;
	
	std::vector<eMusical> m_enemyList;
	std::vector<std::vector<std::shared_ptr<BattleEnemy>>> m_waveEnemyList;
	std::vector<std::vector<eMusical>> m_enemyAttackList;
	std::vector<CharaStatus> m_hp;

	std::unordered_map<eBattleActionType, std::string>m_animationName;
	eBattleActionType m_attackState;

	bool m_bossFlag;
	int m_waveID;
	bool m_isEnd;
	int m_waveAllCount;
	int m_attackAllCount;
	int m_stageID;
	int m_optionCount;
	

};

#endif