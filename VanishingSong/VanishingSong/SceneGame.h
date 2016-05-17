#pragma once
#include<GameScene.h>
#include <DirectXEntity.h>
#include <memory>
#include "ShaderBase.h"
#include "FadeManager.h"
#include "FieldPlayer.h"
#include "CollideManager.h"
class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
		eBattle,
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
	eState m_gameState;
	
	std::unique_ptr<CollideManager> m_pCollideManager;
	std::unique_ptr<FadeManager> m_pFadeObject;
	std::shared_ptr<FieldPlayer> m_pFieldPlayer;
	std::shared_ptr<FieldArea> m_pFieldArea;
};

