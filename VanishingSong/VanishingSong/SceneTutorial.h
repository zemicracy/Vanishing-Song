//#pragma once

#ifndef _SCENETUTORIAL_H
#define _SCENETUTORIAL_H
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
#include "BattleEnemyManager.h"
#include"BattleMessage.h"
#include"GaugeManager.h"
#include"ResultBoard.h"
#include"TutorialMessage.h"
#include"AttackParticle.h"

class SceneTutorial :
	public aetherClass::GameScene
{

public:
	static const std::string Name;
public:
	enum class eGameState{
		ePreCountIn, eCountIn, eUpdate, eFin
	};
	
	SceneTutorial();
	~SceneTutorial();

	//初期化
	bool Initialize()override;
	void Finalize()override;

	//1フレームごとの更新処理
	bool Updater()override;

	//1フレームごとの描画処理
	void Render()override;

	//1フレームごとのui描画処理
	void UIRender()override;

	//トランジション効果を設定
	bool TransitionIn()override;
	bool TransitionOut()override;

private:
	void mCountIn();

	void mOnResult();
	void mOnListen();    // 敵の演奏
	void mOnPerform();   // プレイヤーの演奏
	void mOnBattle();    // 戦闘開始
	void mCheckBattle();

	void mLoadTextData();
	bool mTutorialUpdater();
	void mTimeEngagerForTuto();
	void mRhythmicMotion();
	void mOnDebug();
private:
	GameManager::eBattleState m_battleState;
	GameManager::eBattleState m_winner;
	std::shared_ptr<aetherClass::GameSound> m_sound;

	BattlePlayerManager m_players;
	CharaEntity m_charaEntity;

	std::shared_ptr<ActionBoard>m_pActionBoard;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<BattleField>m_pField;
	std::unique_ptr<BattleMessage>m_pMessage;
	std::unique_ptr<GaugeManager>m_pGauge;
	std::unique_ptr<ResultBoard>m_pResult;
	std::unique_ptr<TutorialMessage>m_pTutorial;
	std::shared_ptr<aetherClass::GameSound>m_pSoundDevice;

	aetherClass::ViewCamera m_view;
	std::shared_ptr<BattleEnemyManager> m_pBattleEnemyManager;



	bool m_initUpdateProcess;
	bool m_preInitProcess;
	bool m_timeEngage;
	bool m_isTutorialPlay;
	bool m_isEndTransition;
	bool m_isPreMessageOpen;

	float m_bgmVolume;
	int m_inCount;
	int m_prevWholeBeatNo;
	int m_countAdlibTiming;
	float m_resultUpdateTime;

	int m_textReadCnt;
	//ウェーブ
	int m_MaxWave;
	int m_waveID;
	int m_stageID;
	GameManager::eGameMode m_beatMode;
	//HPData
	CharaStatus m_charaHp;
	CharaStatus *m_enemyHp;


	eGameState m_processState;

	//Tutorial用
	Tutorial::eTutorialState m_tutorialState;
	std::vector<std::shared_ptr<ActionCommand>>m_enemyVector;

	//パーティクル
	std::shared_ptr<AttackParticle>m_particle;
	AttackParticle::ParticleDesc m_particleDesc;


	//ptr
	std::shared_ptr<RhythmManager>m_rhythm;
};


#endif