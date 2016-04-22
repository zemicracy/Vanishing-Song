#pragma once
#include<GameScene.h>
#include "ShaderBase.h"
#include "EnemyGround.h"
#include "Player.h"
#include "FieldArea.h"
#include "OrderList.h"
#include "ActionBoard.h"
#include <DirectXEntity.h>
class SceneGame :
	public aetherClass::GameScene
{
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
	
	bool mSurvivalUpdate(const float timeScale,const float nowTime);
	bool mPracticeUpdate(const float timeScale, const float nowTime);

	void mSurvivalRender();
	void mPracticeRender();

	void mSurvivalUIRender();
	void mPracticeUIRender();
private:
	std::shared_ptr<EnemyGround> m_penemyGround;
	std::shared_ptr<aetherClass::ShaderBase> m_pixelShader;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<FieldArea> m_pFieldArea;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<ActionBoard> m_pActionBoard;
	aetherClass::DirectXEntity m_directX;
	GameManager::eGameMode m_nowMode;
	GameManager::eDay m_nowDay;

	// �I�������ێ��p
	float m_dayTime;
};

