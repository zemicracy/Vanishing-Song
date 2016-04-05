#ifndef _PLAYER_H
#define _PLAYER_H

#include "GearFrame.h"
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"
#include "ActionCommand.h"
#include <ShaderBase.h>
#include <Transform.h>
#include <WorldReader.h>
#include <ViewCamera.h>
#include <unordered_map>

class Player
{
private:
	enum class eState{
		eMove,
		eWait,
		eNull
	};
public:
	Player();
	~Player();
	/*
		初期化
	*/
	bool mInitialize();

	/*
		更新処理
	*/
	void mUpdate(const float timeScale);

	/*
		描画処理
	*/
	void mRender(aetherClass::ShaderBase* modelShader,aetherClass::ShaderBase* colliderShader);

	/*
		アクション実行用
	*/
	eActionType mAction(std::shared_ptr<ActionCommand>,const float timeScale);

	/*
		実行したものの登録
		第一引数：アクションの種類
		第二引数：何番目のものかの数字
	*/
	void mAddPrevActionCmmand(eActionType,const int id);

	/*
		アクションリストの上書き
	*/
	void mResetPrevActionList();

	/*
		解放処理
	*/
	void mFinalize();

	aetherClass::ViewCamera mGetView();
private:
	/*
		プレイヤーに対するキー入力処理
		現状移動処理と回転処理
	*/
	aetherClass::Transform mReadKey(const float timeScale);

	bool mInitializeGear(std::shared_ptr<GearFrame>&,aetherClass::ViewCamera*);

	bool mLoadModelProperty(std::shared_ptr<GearFrame>&,std::string modelDataFile);

	void mRotationAdjustment(std::shared_ptr<Gear>&);

	void SetLoadModelValue(std::shared_ptr<Gear>&,ObjectInfo*);
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
	std::shared_ptr<ActionCommand> m_pActionCommand;
	std::shared_ptr<Gear> m_pTopGear;
	aetherClass::ViewCamera m_playerView;

	CharaStatus m_status;
	eActionType m_prevAction;
	CharaEntity m_charaEntity;
	eState m_state;

	int m_actionCount;// アクションを行った数を保存しとく用
};

#endif