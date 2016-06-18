#ifndef _BATTLEENEMY_H
#define _BATTLEENEMY_H
#include "Const.h"
#include "ActionBlue.h"
#include "ShaderBase.h"
#include "CharaEntity.h"
#include <memory>
#include<FbxModel.h>
class BattleEnemy
{

public:
	struct Enemy
	{
		std::shared_ptr<aetherClass::FbxModel> _model;
		eMusical _type;
	};

public:
	BattleEnemy();
	~BattleEnemy();

	/*
		����������
	*/
	void mInitialize(eMusical type,eEnemyType enemytype, aetherClass::ViewCamera*,aetherClass::Vector3&);

	Enemy &mGetEnemy();
	eMusical mGetType();

	/*
		�X�V����
		��{�I�ɑҋ@���[�V�������s��
	*/
	void mUpdate(const float timeScale);

	/*
		�U���̃A�j���[�V�����̍Đ�
	*/
	void mOnAttack();

	/*
		�_���[�W�̃A�j���[�V�����̍Đ�
	*/
	void mOnDamage();

	void misDie();

	
	/*
		�`�揈��
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);
private:
	// �������
	// ������GearFrame�͉�����Ȃ�
	void Finalize();


private:
	eMusical m_type;
	Enemy m_enemy;
	bool m_isDie;
	float m_alpha;
};

#endif