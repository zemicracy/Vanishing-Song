#pragma once
#include<GameScene.h>
#include "ShaderBase.h"
#include "EnemyGround.h"
#include "Player.h"
#include "FieldArea.h"
#include "OrderList.h"
#include "ActionBoard.h"
#include "ResultBord.h"
#include "CollideManager.h"
#include "FadeManager.h"
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
	// 初期化処理
	bool mInitalizeShader();
	bool mInitializeMode(GameManager::eGameMode);
	
	bool mSurvivalMainUpdate(const float timeScale,const float nowTime);
	bool mPracticeMainUpdate(const float timeScale, const float nowTime);

	void mSurvivalRender();
	void mPracticeRender();

	void mSurvivalUIRender();
	void mPracticeUIRender();

	void mShowResult(GameManager::eDay, aetherClass::ShaderBase* defaultShader, aetherClass::ShaderBase* bularShader);

	bool mFadeState(eState);
	void mResetProperty(); // 変数の初期化等々

	void mRegisterDayHash(GameManager::eDay key, GameManager::eDay value);
	void mRegisterDay();

	void mFinalizeParam(GameManager::eDay);
	void mInitializeParam(GameManager::eDay);
private:
	std::shared_ptr<EnemyGround> m_penemyGround;
	std::shared_ptr<aetherClass::ShaderBase> m_pixelShader;
	std::shared_ptr<aetherClass::ShaderBase> m_textureShader;

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<FieldArea> m_pFieldArea;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<ActionBoard> m_pActionBoard;
	std::unique_ptr<ResultBord> m_pResultBord;
	std::unique_ptr<FadeManager> m_pFadeObject;
	std::unique_ptr<CollideManager> m_pCollideManager;
	aetherClass::DirectXEntity m_directX;
	GameManager::eGameMode m_nowMode;
	GameManager::eDay m_nowDay;
	std::unordered_map<GameManager::eDay, GameManager::eDay> m_dayHash;
	ResultData m_resultData; // リザルト表示時に使用
	eState m_gameState;
	// 経過時間保持用
	float m_dayTime;
};

