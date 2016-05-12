#pragma once
#include<GameScene.h>
#include <DirectXEntity.h>

#include "ShaderBase.h"
#include "ResultBord.h"
#include "FadeManager.h"

class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
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
	std::unique_ptr<FadeManager> m_pFadeObject;

	eState m_gameState;
	// 経過時間保持用
	float m_dayTime;


};

