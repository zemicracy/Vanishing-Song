#pragma once
#include"ActionBoard.h"
#include"GameSound.h"
#include"GameManager.h"
#include"ActionSound.h"
#include"RhythmManager.h"
#include"AttackParticle.h"
#include"BattleField.h"
#include"ResultBoard.h"
#include"EffectGenerator2D.h"

#include<array>
namespace{
	enum eAppendOption{
		eNone = 0, eBlack = 1, eReverce = 2
	};
}
class OrderList
{
public:

	OrderList();
	~OrderList();
	//再生
	void mPlay();
	void mLinePlay();

	typedef int eAppendoption;	//フラグだけどInt
	//Listen
	void mSetOption(eAppendoption op);
	void mAddEnemyOrder(std::vector<std::shared_ptr<ActionCommand>>&);	//敵からリストを受け取る

	//Perform
	std::shared_ptr<ActionCommand> mGetActionCommand();

	//Check
	//Battle
	int mGetDamage();

	//End
	void mEndReset();

	//Indispensable Method
	void mInitialize(GameManager::eGameMode,GameManager::eBattleState&,ActionBoard*,BattleField*);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdate();

	//AccesserMethod
	bool mIsEnd();
	ResultData mGetResult();

	//other
	void mRender3D(aetherClass::ShaderBase*);

private:
	void mBattleUpdate();
	void mListenUpdate();
	void mPerformUpdate();
	void mLineUpdate();

	void mFinalize();
	
	//音再生
	void mPlaySound(std::shared_ptr<ActionSound>);
	//停止
	void mListStop();
	//リズムに合わせたモーションはここで
	void mRhythmicMotion();
	void mAppendOptionInit();
private:

	//敵とプレイヤーのリスト
	std::vector<std::shared_ptr<ActionCommand>>m_PlayerOrderList;
	std::vector<std::shared_ptr<ActionCommand>>m_EnemyOrderList;

	AttackParticle::ParticleDesc m_perticleDesc;
	std::unique_ptr<AttackParticle>m_pParticle;

	//描画先
	std::vector<std::shared_ptr<aetherClass::SpriteBase>>m_pSpriteList;
	std::vector<aetherClass::Vector3>m_pSpriteOrigin;

	//一時補間場所（外に出す用）
	std::shared_ptr<ActionCommand>m_playedAction;

	//そのたモーション目的
	ActionBoard* m_ActionBoard;
	BattleField* m_Field;
	std::shared_ptr<aetherClass::SpriteBase>m_pVolumeImage;
	aetherClass::Vector3 m_VolumeOrigin;

	std::shared_ptr<aetherClass::SpriteBase>m_pBackImage;
	aetherClass::Vector3 m_BackImageOrigin;
	aetherClass::Vector3 m_BackImageReverceOrigin;
	aetherClass::Vector3 m_BackImageScaleOrigin;

	std::shared_ptr<aetherClass::SpriteBase>m_pReadLine;
	aetherClass::Vector3 m_ReadLineOrigin;
	aetherClass::Vector3 m_ReadLineReverce;

	std::shared_ptr<aetherClass::Rectangle2D>m_pFlame;
	aetherClass::Vector3 m_flamePosOrigin;
	aetherClass::Vector3 m_flameScaleOrigin;


	//リズム
	RhythmManager *m_rhythm;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTextureList;

	//option
	eAppendoption m_option;

	//エフェクト
	std::shared_ptr<EffectGenerator2D> m_pEffect;
	aetherClass::Transform m_effectTrans;

	bool m_isLineStart;		//ライン再生
	int m_eighterCount;		//カウント

	bool m_isAlPlay;		//既に再生後か？
	bool m_isStart;			//判定中か
	bool m_isEnd;			//終了したか

	bool m_isKeyDown;		//その拍ににキーが押されたか
	bool m_isPlaySound;		//コマンド再生dできるか

	int m_processId;		//処理中のID
	int m_MaxOrderSize;
	
	int m_damagedValue;
	ResultData m_resultData;

	//
	GameManager::eBattleState* m_faze;
	GameManager::eGameMode m_mode;
	//定数

	const float m_kMissLevel = 0.5f;
	const float m_kGreat = 0.25f;

};

