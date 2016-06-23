#pragma once
#include<GameScene.h>
#include <DirectXEntity.h>
#include <memory>
#include "ShaderBase.h"
#include "FadeManager.h"
#include "FieldPlayer.h"
#include "CollideManager.h"
#include "MessageManager.h"
#include "AttackParticle.h"
#include "CageManager.h"
#include<GameSound.h>
#include "TutorialEnemy.h"
#include "Config.h"
class SceneGame :
	public aetherClass::GameScene
{
private:

	enum class eState{
		eRun,
		eTutorial,
		ePause,
		eBattle,
		eExit,
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

	void mTutorial(bool ,bool);
	void mRun();
	bool mMessageUpdate();
	void mInitializeBGM();
private:
	aetherClass::DirectXEntity m_directX;	
	eState m_gameState;
	
	std::unique_ptr<CollideManager> m_pCollideManager;
	std::shared_ptr<FieldPlayer> m_pFieldPlayer;
	std::shared_ptr<FieldArea> m_pFieldArea;
	std::shared_ptr<FieldEnemyManager> m_pFieldEnemy;
	std::shared_ptr<MessageManager> m_pMessageManager;
	std::shared_ptr<CageManager> m_pCageManager;
	std::shared_ptr<TutorialEnemy> m_pTutorialEnemy;
	std::vector<std::shared_ptr<aetherClass::GameSound>>m_pBGMArray;
	std::shared_ptr<RhythmManager> m_pRhythm;
	Config m_config;
	bool m_isTransitionEnd;
	aetherClass::DirectXEntity m_directEntity;
	bool m_isFade;
	bool m_isFade2;
	float m_prevVolume;

};

