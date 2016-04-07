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

	//初期化
	bool Initialize()override;

	//解放処理
	void Finalize()override;

	//1フレームごとの更新処理
	bool Updater()override;

	//1フレームごとの描画処理
	void Render()override;

	//UIの描画処理
	void UIRender()override;

	//トランジション効果
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

