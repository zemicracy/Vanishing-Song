#pragma once
#include"ActionBoard.h"
#include"GameSound.h"
#include"GameManager.h"
#include"ActionSound.h"
#include"RhythmManager.h"

#include<array>
class OrderList
{
public:

	OrderList();
	~OrderList();
	//再生
	void mPlay();

	//Listen
	void mAddEnemyOrder(std::vector<std::shared_ptr<ActionCommand>>&);	//敵からリストを受け取る

	//Perform
	void mAddPlayerOrder(std::vector<std::shared_ptr<ActionCommand>>);
	std::shared_ptr<ActionCommand> mGetActionCommand();

	//Check

	//Battle

	//End
	void mEndReset();

	//Indispensable Method
	void mInitialize(GameManager::eGameMode,GameManager::eBattleState&,std::shared_ptr<ActionBoard>);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdate();
	//AccesserMethod

	bool mIsEnd();

private:
	void mBattleUpdate();
	void mListenUpdate();
	void mPerformUpdate();

	void mFinalize();
	
	//音再生
	void mPlaySound(std::shared_ptr<ActionSound>);
	//停止
	void mListStop();
	//リズムに合わせたモーションはここで
	void mRhythmicMotion();

private:

	//敵とプレイヤーのリスト
	std::vector<std::shared_ptr<ActionCommand>>m_PlayerOrderList;
	std::vector<std::shared_ptr<ActionCommand>>m_EnemyOrderList;

	//描画先
	std::vector<std::shared_ptr<aetherClass::SpriteBase>>m_pSpriteList;
	std::vector<aetherClass::Vector3>m_pSpriteOrigin;

	//一時補間場所（外に出す用）
	std::shared_ptr<ActionCommand>m_playedAction;

	//そのたモーション目的
	std::shared_ptr<ActionBoard>m_ActionBoard;
	std::shared_ptr<aetherClass::SpriteBase>m_pVolumeImage;
	aetherClass::Vector3 m_VolumeOrigin;
	std::shared_ptr<aetherClass::SpriteBase>m_pBackImage;
	aetherClass::Vector3 m_BackImageOrigin;
	aetherClass::Vector3 m_BackImageScaleOrigin;

	//リズム
	RhythmManager *m_rhythm;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTextureList;


	bool m_isAlPlay;		//既に再生後か？
	bool m_isStart;			//判定中か
	bool m_isEnd;			//終了したか

	bool m_isKeyDown;		//その拍ににキーが押されたか
	bool m_isPlaySound;		//コマンド再生dできるか

	int m_processId;		//処理中のID
	int m_MaxOrderSize;
	
	int m_enemyDamageCounter;
	int m_playerDamageCounter;



	//
	GameManager::eBattleState* m_faze;
	GameManager::eGameMode m_mode;
	//定数

	const float m_kMissLevel = 0.5f;
	const float m_kGreat = 0.3f;

};

