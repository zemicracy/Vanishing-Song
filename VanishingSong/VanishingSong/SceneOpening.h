//#pragma once
#ifndef _SCENEGAME_H
#define _SCENEGAME_H

#include<GameScene.h>
#include<ShaderBase.h>
#include<SpriteBase.h>
#include<Texture.h>
#include<memory>
#include<GameClock.h>
#include"GameSound.h"

class SceneOpening :
	public aetherClass::GameScene
{
public:
	static const std::string Name;
private:
	std::unique_ptr<aetherClass::SpriteBase>m_pSpriteBase;

	//BGM�p
	std::shared_ptr<aetherClass::GameSound> m_openingSound;

	//�e�N�X�`���p�̔z��
	std::unique_ptr<aetherClass::Texture>m_pTexture[17];

	//���ԃJ�E���g�p�ϐ�
	double m_clockCount;

	//�z��ԍ��̐؂芷���p�ϐ�
	int m_array;

	//�e�N�X�`���\���J��Ԃ��p�ϐ�
	int m_imageCount;
	std::shared_ptr<aetherClass::SpriteBase> m_pSkip;
	aetherClass::Texture m_skipTexture;

	//�e�N�X�`���J��Ԃ��\���Ƒ����ĕ\������p�^�[���A�؂�ւ��p�̕ϐ�
	int m_openingflg;

	bool m_isInit;
public:
	SceneOpening();
	~SceneOpening();

	//������
	bool Initialize()override;
	void Finalize()override;

	//1�t���[�����Ƃ̍X�V����
	bool Updater()override;

	//1�t���[�����Ƃ̕`�揈��
	void Render()override;

	//1�t���[�����Ƃ�ui�`�揈��
	void UIRender()override;

	//�g�����W�V�������ʂ�ݒ�
	bool TransitionIn()override;
	bool TransitionOut()override;
};

#endif