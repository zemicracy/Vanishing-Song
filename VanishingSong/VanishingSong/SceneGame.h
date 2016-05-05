#pragma once
#include<GameScene.h>
#include "ShaderBase.h"
#include "ResultBord.h"
#include "FadeManager.h"
#include "Mode.h"
#include <DirectXEntity.h>
class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
		ePause,
		eExit,
		eResult,
		eGameOver,
		eFadeIn,
		eShowDay,
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
	
	std::shared_ptr<Mode> mReturnMode(GameManager::eGameMode);

	void mShowResult(GameManager::eDay, aetherClass::ShaderBase* defaultShader, aetherClass::ShaderBase* bularShader);

	bool mFadeState(eState);
	void mResetProperty(); // �ϐ��̏��������X

	void mRegisterDayHash(GameManager::eDay key, GameManager::eDay value);
	void mRegisterDay();
private:

	std::unique_ptr<ResultBord> m_pResultBord;
	std::unique_ptr<FadeManager> m_pFadeObject;

	std::shared_ptr<Mode> m_pMode;
	aetherClass::DirectXEntity m_directX;

	GameManager::eDay m_day;
	std::unordered_map<GameManager::eDay, GameManager::eDay> m_dayHash;
	ResultData m_resultData; // ���U���g�\�����Ɏg�p
	eState m_gameState;
	// �o�ߎ��ԕێ��p
	float m_dayTime;


};

