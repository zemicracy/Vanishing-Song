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
public:
	static const std::string Name;
private:
	// 初期化処理
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

	// 終了時刻保持用
	float m_dayTime;
};

