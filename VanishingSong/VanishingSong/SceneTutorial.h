#ifndef SCENETUTORIAL_H
#define SCENETUTORIAL_H
#include <GameScene.h>
class SceneTutorial:
	public aetherClass::GameScene
{
public:
	SceneTutorial();
	~SceneTutorial();

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

public:
	static const std::string Name;
};

#endif