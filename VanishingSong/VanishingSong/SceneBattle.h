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

class SceneBattle :
	public aetherClass::GameScene
{

public:
	static const std::string Name;
public:
	enum class eGameState{
		ePreCountIn,eCountIn,eUpdate,eFin
	};

	SceneBattle();
	~SceneBattle();

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
private:
	GameManager::eBattleState m_battleState;
	GameManager::eBattleState m_winner;


	BattlePlayerManager m_players;
	CharaEntity m_charaEntity;
	std::shared_ptr<ActionBoard>m_pActionBoard;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<BattleField>m_pField;
	std::unique_ptr<BattleMessage>m_pMessage;
	std::unique_ptr<GaugeManager>m_pGauge;
	std::unique_ptr<ResultBoard>m_pResult;

	aetherClass::ViewCamera m_view;
	std::shared_ptr<BattleEnemyManager> m_pBattleEnemyManager;

	int m_prevWholeBeatNo;
	bool m_InitUpdateProcess;
	bool m_PreInitProcess;
	float m_bgmVolume;
	int m_inCount;
	//karikari
	CharaStatus charaHp;
	CharaStatus enemyHp;


	eGameState m_processState;

	//ptr
	RhythmManager *m_rhythm;
};


#endif