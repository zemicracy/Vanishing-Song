#pragma once
#include<GameScene.h>
#include <DirectXEntity.h>
#include <memory>
#include "ShaderBase.h"
#include "FadeManager.h"
#include "FieldPlayer.h"
#include "CollideManager.h"
#include "MessageManager.h"
#include "AttackParticle.h"
#include "CageManager.h"
#include<GameSound.h>
#include "TutorialEnemy.h"
class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
		eTutorial,
		ePause,
		eBattle,
		eExit,
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

	void mTutorial();
	void mRun();
	bool mMessageUpdate();
private:
	aetherClass::DirectXEntity m_directX;	
	eState m_gameState;
	
	std::unique_ptr<CollideManager> m_pCollideManager;
	std::shared_ptr<FieldPlayer> m_pFieldPlayer;
	std::shared_ptr<FieldArea> m_pFieldArea;
	std::shared_ptr<FieldEnemyManager> m_pFieldEnemy;
	std::shared_ptr<MessageManager> m_pMessageManager;
	std::shared_ptr<CageManager> m_pCageManager;
	std::shared_ptr<TutorialEnemy> m_pTutorialEnemy;
	std::vector<std::shared_ptr<aetherClass::GameSound>>m_pBGMArray;
	bool m_isTransitionEnd;
};

