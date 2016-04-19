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

	
	struct AnimationFrame{
		std::vector<Animation> _animation;
		int _animationFrame;
	};

	
	struct KeyValues{
		Player::eState _state;
		aetherClass::Transform _transform;
		aetherClass::Vector3 _cameraRotation;
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

	/*
		カメラオブジェクトのアドレス取得用
	*/
	aetherClass::ViewCamera* mGetView();

	/*
		コライダーの取得用
	*/
	std::shared_ptr<aetherClass::Cube> mGetBodyColldier();
private:
	
	/*
		カメラオブジェクトの初期化
	*/
	void mInitialPlayerView(CameraValue);

	/*
		全てのギアの初期化
	*/
	bool mInitializeGearFrame(std::shared_ptr<GearFrame>&, aetherClass::ViewCamera*);

	/*
		エディターからの値を読み取るよう
	*/
	bool mLoadProperty(std::shared_ptr<GearFrame>&, std::string modelDataFile);
	
	/*
		コライダーの初期化
	*/
	void mSetUpBodyCollider(std::shared_ptr<aetherClass::Cube>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);

	/*
		コライダーの更新処理
	*/
	void mUpdateBodyCollider(aetherClass::Transform&);
	/*
		アニメーションの登録
	*/
	void mRegisterAnimation(Player::eState key,const int allFrame, std::string first, std::string last);

	/*
		アニメーション再生用
	*/
	void mDefaultAnimation(Player::eState& state);

	/*
		カメラオブジェクトの更新
	*/
	void mUpdateView(aetherClass::ViewCamera&,aetherClass::Vector3& rotation,aetherClass::Vector3 lookAtPosition);

	/*
	キーやマウスの処理の読み取り
	*/
	KeyValues mReadKey(const float timeScale);


private:
	std::shared_ptr<GearFrame> m_pGearFrame;   // パーツの管理
	std::shared_ptr<ActionCommand> m_pActionCommand;  // コマンド実行用
	std::shared_ptr<Gear> m_pTopGear;            // 最上位パーツのポインタを入れておく
	aetherClass::ViewCamera m_playerView;		//　カメラオブジェクト

	aetherClass::Transform m_playerTransform;   // プレイヤーの回転、移動、スケールを管理
	aetherClass::Vector3 m_cameraRotation;		//　カメラの回転を管理

	CharaStatus m_status;                      // プレイヤーのステータス
	eCommandType m_prevCommand;					// 前回実行したコマンドの種類
	eState m_prevState;							// 前回のプレイヤーの状態
	CharaEntity m_charaEntity;					// 便利関数のあるクラスオブジェクト
	Offset m_cameraOffset;						//　カメラのオフセット
	Counter m_actionCount;			// それぞれのアクションを行ったフレーム数を保存しとく用

	std::shared_ptr<aetherClass::Cube> m_pBodyCollider;   // 基本的なコライダー

	std::unordered_map<eState, AnimationFrame> m_defaultAnimation;   // 基本的なアニメーションの値を含んだ連想配列

	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_pGearPartsHash;   // それぞれのギアのポインタを扱いやすいようにまとめた連想配列
};

#endif