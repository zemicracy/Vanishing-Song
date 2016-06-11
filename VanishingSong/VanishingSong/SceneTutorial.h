//#pragma once

#ifndef _SCENEBATTLE_H
#define _SCENEBATTLE_H
#include <GameScene.h>
#include <ModelBase.h>
#include <ShaderBase.h>
#include <memory>
#include <ViewCamera.h>
#include <Texture.h>
#include"Skybox.h"

#include "GameManager.h"
#include"OrderList.h"
#include"ActionBoard.h"
#include"BattleField.h"
#include "BattlePlayerManager.h"
#include "BattleEnemyManager.h"
#include"BattleMessage.h"
#include"GaugeManager.h"
#include"ResultBoard.h"
#include"TutorialMessage.h"

class SceneTutorial :
	public aetherClass::GameScene
{

public:
	static const std::string Name;
public:
	enum class eGameState{
		ePreCountIn, eCountIn, eUpdate, eFin
	};


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

private:
	void mCountIn();

	void mOnResult();
	void mOnListen();    // �G�̉��t
	void mOnPerform();   // �v���C���[�̉��t
	void mOnBattle();    // �퓬�J�n
	void mCheckBattle();

	void mLoadTextData();
	bool mTutorialUpdater();
	void mTimeEngagerForTuto();
	void mRhythmicMotion();
private:
	GameManager::eBattleState m_battleState;
	GameManager::eBattleState m_winner;
	std::shared_ptr<aetherClass::GameSound> m_sound;

	BattlePlayerManager m_players;
	CharaEntity m_charaEntity;

	std::shared_ptr<ActionBoard>m_pActionBoard;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<BattleField>m_pField;
	std::unique_ptr<BattleMessage>m_pMessage;
	std::unique_ptr<GaugeManager>m_pGauge;
	std::unique_ptr<ResultBoard>m_pResult;
	std::unique_ptr<TutorialMessage>m_pTutorial;

	aetherClass::ViewCamera m_view;
	std::shared_ptr<BattleEnemyManager> m_pBattleEnemyManager;



	bool m_initUpdateProcess;
	bool m_preInitProcess;
	bool m_timeEngage;
	bool m_isTutorialPlay;

	float m_bgmVolume;
	int m_inCount;
	int m_prevWholeBeatNo;

	int m_textReadCnt;
	//�E�F�[�u
	int m_MaxWave;
	int m_waveID;
	int m_stageID;
	GameManager::eGameMode m_beatMode;
	//HPData
	CharaStatus m_charaHp;
	CharaStatus *m_enemyHp;


	eGameState m_processState;

	//Tutorial�p
	eTutorialState m_tutorialState;
	std::vector<std::shared_ptr<ActionCommand>>m_enemyVector;


	//ptr
	std::shared_ptr<RhythmManager>m_rhythm;
};


#endif