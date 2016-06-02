//#pragma once
#ifndef _SCENEGAME_H
#define _SCENEGAME_H

#include<GameScene.h>
#include<ShaderBase.h>
#include<ViewCamera.h>
#include<SpriteBase.h>
#include<Texture.h>
#include<memory>
#include<GameClock.h>

//#include<OpeningPicture.h>

class Opening:
	public aetherClass::GameScene
{
private:
	std::unique_ptr<aetherClass::ShaderBase> m_pShaderBase;
	std::unique_ptr<aetherClass::ViewCamera> m_pOpeningView;
	//std::unique_ptr<aetherClass::SpriteBase>m_pSpriteBase;
	std::unique_ptr<aetherClass::ModelBase>m_pModelBase;
	std::unique_ptr<aetherClass::Texture>m_pTexture[8];
	double m_clockCount;
	int m_array;
	int m_imageCount;
public:
	Opening();
	~Opening();

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