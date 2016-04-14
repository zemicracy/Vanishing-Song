//#pragma once

#ifndef _SCENESKILL_H
#define _SCENESKILL_H
#include<GameScene.h>
#include<ModelBase.h>
#include<ShaderBase.h>
#include<memory>
#include<ViewCamera.h>
#include<Texture.h>

class SceneSkill :
	public aetherClass::GameScene
{
private:
	std::unique_ptr<aetherClass::ModelBase> m_pModelBase;
	std::unique_ptr<aetherClass::ShaderBase> m_pShaderBase;
	std::unique_ptr<aetherClass::ViewCamera> m_pTitleView;
	std::unique_ptr<aetherClass::Texture>m_pTexture;
public:
	SceneSkill();
	~SceneSkill();

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