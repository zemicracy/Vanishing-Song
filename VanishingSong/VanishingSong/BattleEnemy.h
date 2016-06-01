#ifndef _BATTLEENEMY_H
#define _BATTLEENEMY_H
#include "Const.h"
#include "GearFrame.h"
#include "ActionBlue.h"
#include "ShaderBase.h"
#include "CharaEntity.h"
#include <memory>
class BattleEnemy
{

public:
	struct Enemy
	{
		std::shared_ptr<GearFrame> _gearFrame;
		eMusical _type;
	};

public:
	BattleEnemy();
	~BattleEnemy();

	/*
		����������
	*/
	void mInitialize(eMusical type, aetherClass::ViewCamera*,aetherClass::Vector3&);

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

	
	/*
		�`�揈��
	*/
	void mRender(std::shared_ptr<aetherClass::ShaderBase>);
private:
	// �������
	// ������GearFrame�͉�����Ȃ�
	void Finalize();
	bool mInitializeBlue(aetherClass::ViewCamera*, aetherClass::Vector3& pos);
	bool mInitializeGreen(aetherClass::ViewCamera*, aetherClass::Vector3& pos);
	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);


private:
	eMusical m_type;
	Enemy m_enemy;
	CharaEntity m_charaEntity;;
	std::shared_ptr<GearFrame> m_GearFrame;
	std::shared_ptr<Gear> m_pTopGear;

};

#endif