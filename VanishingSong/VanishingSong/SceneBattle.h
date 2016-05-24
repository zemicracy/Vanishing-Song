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

	void mOnListen();    // �G�̉��t
	void mOnPerform();   // �v���C���[�̉��t
	void mOnBattle();    // �퓬�J�n
	void mCheckBattle();
private:
	GameManager::eBattleState m_battleState;

	BattlePlayerManager m_players;

	std::shared_ptr<ActionBoard>m_pActionBoard;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<BattleField>m_pField;
	aetherClass::ViewCamera m_view;

	int m_prevWholeBeatNo;
	bool m_InitUpdateProcess;

	eGameState m_processState;

	//ptr
	RhythmManager *m_rhythm;
};


#endif