#pragma once
#include<GameScene.h>
#include <DirectXEntity.h>

#include "ShaderBase.h"
#include "ResultBord.h"
#include "FadeManager.h"

class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
		ePause,
		eExit,
		eFadeIn,
		eFadeOut,
		eNull
	};
	
public:
	SceneGame();
	~SceneGame();

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
public:
	static const std::string Name;

private:
	bool mFadeState(eState);
	void mResetProperty(); // �ϐ��̏��������X

private:
	aetherClass::DirectXEntity m_directX;
	std::unique_ptr<FadeManager> m_pFadeObject;

	eState m_gameState;
	// �o�ߎ��ԕێ��p
	float m_dayTime;


};

