#pragma once
#include<GameScene.h>
#include "ShaderBase.h"
#include "EnemyGround.h"
#include "Player.h"
#include "FieldArea.h"
#include "OrderList.h"
#include "ActionBoard.h"
#include "ResultBord.h"
#include "CollideManager.h"
#include "FadeManager.h"
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
	// ����������
	bool mInitalizeShader();
	bool mInitializeMode(GameManager::eGameMode);
	
	bool mSurvivalMainUpdate(const float timeScale,const float nowTime);
	bool mPracticeMainUpdate(const float timeScale, const float nowTime);

	void mSurvivalRender();
	void mPracticeRender();

	void mSurvivalUIRender();
	void mPracticeUIRender();

	void mShowResult(GameManager::eDay, aetherClass::ShaderBase* defaultShader, aetherClass::ShaderBase* bularShader);

	bool mFadeState(eState);
	void mResetProperty(); // �ϐ��̏��������X

	void mRegisterDayHash(GameManager::eDay key, GameManager::eDay value);
	void mRegisterDay();

	void mFinalizeParam(GameManager::eDay);
	void mInitializeParam(GameManager::eDay);
private:
	std::shared_ptr<EnemyGround> m_penemyGround;
	std::shared_ptr<aetherClass::ShaderBase> m_pixelShader;
	std::shared_ptr<aetherClass::ShaderBase> m_textureShader;

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<FieldArea> m_pFieldArea;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<ActionBoard> m_pActionBoard;
	std::unique_ptr<ResultBord> m_pResultBord;
	std::unique_ptr<FadeManager> m_pFadeObject;
	std::unique_ptr<CollideManager> m_pCollideManager;
	aetherClass::DirectXEntity m_directX;
	GameManager::eGameMode m_nowMode;
	GameManager::eDay m_nowDay;
	std::unordered_map<GameManager::eDay, GameManager::eDay> m_dayHash;
	ResultData m_resultData; // ���U���g�\�����Ɏg�p
	eState m_gameState;
	// �o�ߎ��ԕێ��p
	float m_dayTime;
};

