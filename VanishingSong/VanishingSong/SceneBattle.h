//#pragma once

#ifndef _SCENEBATTLE_H
#define _SCENEBATTLE_H
#include <GameScene.h>
#include <ModelBase.h>
#include <ShaderBase.h>
#include <memory>
#include <ViewCamera.h>
#include <Texture.h>
#include "GameManager.h"
#include"OrderList.h"
class SceneBattle :
	public aetherClass::GameScene
{

public:
	static const std::string Name;
public:
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
	void OnListen();    // �G�̉��t
	void OnPerform();   // �v���C���[�̉��t
	void OnBattle();    // �퓬�J�n
	void CheckBattle();
private:
	GameManager::eBattleState m_battleState;
	std::unique_ptr<aetherClass::ModelBase> m_pModelBase;
	std::unique_ptr<aetherClass::Texture>m_pTexture;

	std::unique_ptr<OrderList> m_pOrderList;


	aetherClass::ViewCamera m_view;
};


#endif