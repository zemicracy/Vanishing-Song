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
		���������̈����łǂ̓G��z�u���邩���擾����
		_type��Null�̏ꍇ�͏������̏������΂�
	*/
	void Initialize(aetherClass::ViewCamera* camera,BattleField* );

	void mUpadate(const float timeScale);

	std::vector<eMusical> GetList();

	void AddList();

	/*
		�`�揈��
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);

	/*
		�G���S�Ŏ���true��Ԃ��悤�ɂ���
	*/
	bool mIsEnd();

private:
	void mLoadInfo(std::string);
private:
	
	void BlueAdd();
	void RedAdd();
	void GreenAdd();

	bool m_isEnd;
	std::vector<std::shared_ptr<BattleEnemy>> m_pEnemy;
	BattleField* m_BattleField;
	std::vector<eMusical> m_enemyList;

};

#endif