#pragma once
#include<GameScene.h>
#include "ShaderBase.h"
#include "EnemyGround.h"
#include "Player.h"
#include "FieldArea.h"
#include "OrderList.h"
#include "ActionBoard.h"
class SceneSurvival :
	public aetherClass::GameScene
{
public:
	SceneSurvival();
	~SceneSurvival();

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

private:
	std::shared_ptr<EnemyGround> m_penemyGround;
	std::shared_ptr<aetherClass::ShaderBase> m_pixelShader;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<FieldArea> m_pFieldArea;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<ActionBoard> m_pActionBoard;

};

