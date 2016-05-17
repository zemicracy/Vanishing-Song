#ifndef _BATTLEENEMYMANAGER_H
#define _BATTLEENEMYMANAGER_H
#include "BattleEnemy.h"
#include <ShaderBase.h>
#include <array>
namespace{
	const int kEnemyMaxNum = 4;
}
class BattleEnemyManager
{
public:
	struct Enemy
	{
		std::shared_ptr<GearFrame> _gearFrame;
		eMusical _type;
	};
public:
	BattleEnemyManager();
	~BattleEnemyManager();
	/*
		���������̈����łǂ̓G��z�u���邩���擾����
		_type��Null�̏ꍇ�͏������̏������΂�
	*/
	void Initialize(std::array<Enemy, kEnemyMaxNum> enemyList);

	/*
		�ǂ̉��𗬂������l������
		�������X�g�ɃX�^�b�N����Ƃ��ɑΏۂ̉���enum��Ԃ�
		����ȊO��eMusical::Null��Ԃ�
	*/
	eMusical mUpadate(const float timeScale);

	/*
		�`�揈��
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);

	/*
		�G���S�Ŏ���true��Ԃ��悤�ɂ���
	*/
	bool mIsEnd();

private:
	std::array<BattleEnemy, kEnemyMaxNum> m_enemyArray;
	bool m_isEnd;
};

#endif