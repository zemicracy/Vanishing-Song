#pragma once
#include<GameScene.h>
#include <DirectXEntity.h>
#include <memory>
#include "ShaderBase.h"
#include "FadeManager.h"
#include "FieldPlayer.h"
#include "CollideManager.h"
class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
		eBattle,
		ePause,
		eExit,
		eFadeIn,
		eFadeOut,
		eNull
	};
	
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
	bool mFadeState(eState);
	void mResetProperty(); // 変数の初期化等々

private:
	aetherClass::DirectXEntity m_directX;	
	eState m_gameState;
	
	std::unique_ptr<CollideManager> m_pCollideManager;
	std::unique_ptr<FadeManager> m_pFadeObject;
	std::shared_ptr<FieldPlayer> m_pFieldPlayer;
	std::shared_ptr<FieldArea> m_pFieldArea;
};

