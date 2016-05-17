#ifndef _BATTLEENEMY_H
#define _BATTLEENEMY_H
#include "Const.h"
#include "GearFrame.h"
#include "ShaderBase.h"
#include <memory>
class BattleEnemy
{
public:
	BattleEnemy();
	~BattleEnemy();

	/*
		����������
	*/
	void mInitialize(eMusical type, std::shared_ptr<GearFrame>);
	eMusical mGetType();

	/*
		�X�V����
		��{�I�ɑҋ@���[�V�������s��
	*/
	void mUpDate(const float timeScale);

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
private:
	eMusical m_type;
	std::shared_ptr<GearFrame> m_GearFrame;
};

#endif