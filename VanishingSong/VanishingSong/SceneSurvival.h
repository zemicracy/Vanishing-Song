#pragma once
#include<GameScene.h>
class SceneSurvival :
	public aetherClass::GameScene
{
public:
	SceneSurvival();
	~SceneSurvival();

	//������
	bool Initialize()override;

	//�������
	void Finalize()override;

	//1�t���[�����Ƃ̍X�V����
	bool Updater()override;

	//1�t���[�����Ƃ̕`�揈��
	void Render()override;

	//UI�̕`�揈��
	void UIRender()override;

	//�g�����W�V��������
	bool TransitionIn()override;
	bool TransitionOut()override;
};

