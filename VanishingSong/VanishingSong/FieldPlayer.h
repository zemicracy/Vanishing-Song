#ifndef _FIELDPLAYER_H
#define _FIELDPLAYER_H

#include <Cube.h>
#include <Sphere.h>
#include <ShaderBase.h>
#include <Transform.h>
#include <ViewCamera.h>
#include <unordered_map>
#include <vector>
#include "GearFrame.h"
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"
#include "Animation.h"

class FieldPlayer
{
private:

	/*	FieldPlayerの状態		*/
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
		bool _changeDefaultFrame;
	private:
		void Reset(){
			_defaultFrame = NULL;
			_changeDefaultFrame = false;
		}
	};
	
	struct KeyValues{

		aetherClass::Transform _transform;
		aetherClass::Vector3 _cameraRotation;
	};

public:
	FieldPlayer() = default;
	~FieldPlayer();
	/*
	初期化
	*/
	bool mInitialize(std::shared_ptr<GearFrame> gearFrame,aetherClass::Vector3 position);

	/*
	更新処理
	*/
	void mUpdate(const float timeScale, const bool isWait);

	/*
	描画処理
	*/
	void mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader);
	/*
		カメラオブジェクトのアドレス取得用
	*/
	aetherClass::ViewCamera* mGetView();

	/*
		コライダーの取得用
	*/
	std::shared_ptr<aetherClass::Cube> mGetBodyColldier();
	std::shared_ptr<aetherClass::Sphere> mGetSphereColldier();
	// 壁に当たった時の処理
	void mOnHitWall();
	
	void mSetFieldNumber(const int);
	int mGetFieldNumber()const; 
	void mSetTransform(aetherClass::Transform);
	aetherClass::Transform mGetTransform();
private:


	/*
	解放処理
	*/
	void mFinalize();

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
	void mSetUpBodyCollider(std::shared_ptr<aetherClass::Sphere>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);

	/*
		コライダーの更新処理
	*/
	void mUpdateBodyCollider(aetherClass::Transform&);
	/*
		アニメーションの登録
	*/
	void mRegisterAnimation(FieldPlayer::eState key,const int allFrame, std::string first, std::string last);

	/*
		アニメーション再生用
	*/
	void mDefaultAnimation(FieldPlayer::eState& state);

	/*
		カメラオブジェクトの更新
	*/
	void mUpdateView(aetherClass::ViewCamera&,aetherClass::Vector3& rotation,aetherClass::Vector3 lookAtPosition);

	/*
	キーやマウスの処理の読み取り
	*/
	KeyValues mReadKey(const float timeScale);
	
	void mCheckCameraRotation(aetherClass::Vector3&);
private:
	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_pGearHash;   // それぞれのギアのポインタを扱いやすいようにまとめた連想配列

	std::shared_ptr<Gear> m_topGear;            // 最上位パーツのポインタを入れておく

	bool m_isHitWall;
	aetherClass::ViewCamera m_playerView;		//　カメラオブジェクト

	aetherClass::Transform m_playerTransform;   // プレイヤーの回転、移動、スケールを管理
	aetherClass::Vector3 m_prevPosition;     // 前回のトランスフォーム情報
	aetherClass::Transform m_initialTransform; 

	aetherClass::Vector3 m_cameraRotation;		//　カメラの回転を管理

	eState m_prevState;							// 前回のプレイヤーの状態
	CharaEntity m_charaEntity;					// 便利関数のあるクラスオブジェクト
	Offset m_cameraOffset;						//　カメラのオフセット
	Counter m_actionCount;			// それぞれのアクションを行ったフレーム数を保存しとく用
	std::shared_ptr<aetherClass::Cube> m_pBodyCollider;   // 基本的なコライダー
	std::shared_ptr<aetherClass::Sphere> m_pSphereCollider;   // 基本的なコライダー
	std::unordered_map<eState, AnimationFrame> m_defaultAnimation;   // 基本的なアニメーションの値を含んだ連想配列
	eActionType m_action;

	int m_fieldNumber;
	
};

#endif