
#ifndef _SCENECREGIT_H
#define _SCENECREGIT_H
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
#include"AttackParticle.h"
#include"CregitMessage.h"

class SceneCregit :
	public aetherClass::GameScene
{

public:
	static const std::string Name;
public:
	enum class eGameState{
		ePreCountIn, eCountIn, eUpdate, eFin
	};

	SceneCregit();
	~SceneCregit();

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

	void mOnListen();    // 敵の演奏
	void mOnPerform();   // プレイヤーの演奏
	void mOnBattle();    // 戦闘開始
	void mCheckBattle();
	void mOnResult();

	void mLoadTextData();
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
	std::unique_ptr<CregitMessage>m_pCregitMessage;

	aetherClass::ViewCamera m_view;
	std::shared_ptr<BattleEnemyManager> m_pBattleEnemyManager;


	int m_prevWholeBeatNo;
	bool m_initUpdateProcess;
	bool m_isEndTransition;

	bool m_preInitProcess;
	float m_bgmVolume;
	int m_inCount;

	//ウェーブ
	int m_MaxWave;
	int m_waveID;
	int m_stageID;
	GameManager::eGameMode m_beatMode;

	//パーティクル
	std::shared_ptr<AttackParticle>m_particle;
	AttackParticle::ParticleDesc m_particleDesc;


	eGameState m_processState;
	std::vector<std::shared_ptr<ActionCommand>>m_enemyVector;
	
	std::shared_ptr<aetherClass::SpriteBase>m_pBackCover;
	std::shared_ptr<aetherClass::Texture>m_pBackTexture;

	
	//ptr
	std::shared_ptr<RhythmManager>m_rhythm;
};


#endif