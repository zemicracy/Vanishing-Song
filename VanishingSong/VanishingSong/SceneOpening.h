//#pragma once
#ifndef _SCENEGAME_H
#define _SCENEGAME_H

#include<GameScene.h>
#include<ShaderBase.h>
#include<SpriteBase.h>
#include<Texture.h>
#include<memory>
#include<GameClock.h>

class SceneOpening :
	public aetherClass::GameScene
{
private:
	std::unique_ptr<aetherClass::ShaderBase> m_pShaderBase;
	std::unique_ptr<aetherClass::SpriteBase>m_pSpriteBase;

	//�e�N�X�`���p�̔z��
	std::unique_ptr<aetherClass::Texture>m_pTexture[16];

	double m_clockCount;
	int m_array;
	int m_imageCount;
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