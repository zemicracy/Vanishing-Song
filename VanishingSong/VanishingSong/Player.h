#ifndef _PLAYER_H
#define _PLAYER_H

#include "GearFrame.h"
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"
#include "ActionCommand.h"
#include "Animation.h"
#include <Cube.h>
#include <ShaderBase.h>
#include <Transform.h>
#include <WorldReader.h>
#include <ViewCamera.h>
#include <unordered_map>
#include <vector>
class Player
{
private:
	/*	Playerの状態		*/
	enum class eState{
		eMove,
		eWait,
		eNull
	};

	/*	とりあえずカメラ用	*/
	struct Offset
	{
		aetherClass::Vector3 _translation;
		aetherClass::Vector3 _rotation;
	};


	struct Counter{
		Counter(){
			Reset();
		}

		~Counter(){
			Reset();
		}
		int _defaultFrame;
		int _commandFrame;
		bool _changeDefaultFrame;
		bool _changeCommandFrame;
	private:
		void Reset(){
			_defaultFrame = NULL;
			_commandFrame = NULL;
			_changeDefaultFrame = false;
			_changeCommandFrame = false;
		}
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
	void mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader);

	/*
	アクション実行用
	*/
	eCommandType mCommand(std::shared_ptr<ActionCommand>, const float timeScale);

	/*
	実行したものの登録
	第一引数：アクションの種類
	第二引数：何番目のものかの数字
	*/
	void mAddPrevActionCmmand(eCommandType, const int id);

	/*
	アクションリストの上書き
	*/
	void mResetPrevActionList();

	/*
	解放処理
	*/
	void mFinalize();

	aetherClass::ViewCamera* mGetView();

	std::shared_ptr<aetherClass::Cube> mGetColldier();
private:
	/*
	プレイヤーに対するキー入力処理
	現状移動処理と回転処理
	*/
	aetherClass::Transform mReadKey(const float timeScale);

	bool mInitializeGear(std::shared_ptr<GearFrame>&, aetherClass::ViewCamera*);

	bool mLoadModelProperty(std::shared_ptr<GearFrame>&, std::string modelDataFile);

	void mRotationAdjustment(std::shared_ptr<Gear>&);

	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);

	void mSetUpCollider(std::shared_ptr<aetherClass::Cube>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);

	void mRegisterAnimation(Player::eState key, std::string first, std::string last);

	void mGetAnimationTransform(Player::eState state,aetherClass::Transform);

	void mRegisterParts(Gear::eType, std::shared_ptr<Gear>&);
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
	std::shared_ptr<ActionCommand> m_pActionCommand;
	std::shared_ptr<Gear> m_pTopGear;
	aetherClass::ViewCamera m_playerView;

	CharaStatus m_status;
	eCommandType m_prevCommand;
	eState m_prevState;
	CharaEntity m_charaEntity;
	Offset m_cameraOffset;
	aetherClass::Vector3 m_cameraRotation;
	Counter m_actionCount;			// アクションを行った数を保存しとく用

	std::shared_ptr<aetherClass::Cube> m_pCubeCollider;

	std::unordered_map<eState, std::vector<Animation>> m_defaultAnimation;

	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_pGearPartsHash;
};

#endif