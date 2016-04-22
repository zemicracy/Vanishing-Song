#ifndef _SCENEPRACTICE_H
#define _SCENEPRACTICE_H
#include <GameScene.h>
class ScenePractice :
	public aetherClass::GameScene
{
public:
	ScenePractice();
	~ScenePractice();

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