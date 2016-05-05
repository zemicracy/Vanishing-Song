#pragma once
#include<GameScene.h>
#include "ShaderBase.h"
#include "ResultBord.h"
#include "FadeManager.h"
#include "Mode.h"
#include <DirectXEntity.h>
class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
		ePause,
		eExit,
		eResult,
		eGameOver,
		eFadeIn,
		eShowDay,
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
	
	std::shared_ptr<Mode> mReturnMode(GameManager::eGameMode);

	void mShowResult(GameManager::eDay, aetherClass::ShaderBase* defaultShader, aetherClass::ShaderBase* bularShader);

	bool mFadeState(eState);
	void mResetProperty(); // 変数の初期化等々

	void mRegisterDayHash(GameManager::eDay key, GameManager::eDay value);
	void mRegisterDay();
private:

	std::unique_ptr<ResultBord> m_pResultBord;
	std::unique_ptr<FadeManager> m_pFadeObject;

	std::shared_ptr<Mode> m_pMode;
	aetherClass::DirectXEntity m_directX;

	GameManager::eDay m_day;
	std::unordered_map<GameManager::eDay, GameManager::eDay> m_dayHash;
	ResultData m_resultData; // リザルト表示時に使用
	eState m_gameState;
	// 経過時間保持用
	float m_dayTime;


};

