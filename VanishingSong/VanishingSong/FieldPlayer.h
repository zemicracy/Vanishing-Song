#ifndef _FIELDPLAYER_H
#define _FIELDPLAYER_H

#include <Cube.h>
#include <Sphere.h>
#include <ShaderBase.h>
#include <Transform.h>
#include <ViewCamera.h>
#include <unordered_map>
#include <vector>
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"

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

public:
	FieldPlayer() = default;
	~FieldPlayer();
	/*
	初期化
	*/
	bool mInitialize(std::shared_ptr<aetherClass::FbxModel> model,aetherClass::Transform trans);

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
	// 壁に当たった時の処理
	void mOnHitWall();
	
	void mSetFieldNumber(const int);
	int mGetFieldNumber()const; 
	aetherClass::Transform mGetTransform();
private:
	/*
	解放処理
	*/
	void mFinalize();

	/*
		カメラオブジェクトの初期化
	*/
	void mInitialPlayerView(CameraValue,aetherClass::Vector3);

	/*
		コライダーの初期化
	*/

	void mSetUpBodyCollider(std::shared_ptr<aetherClass::Cube>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);
	
	/*
		コライダーの更新処理
	*/
	void mUpdateBodyCollider(aetherClass::Transform&);
	/*
		カメラオブジェクトの更新
	*/
	void mUpdateView(aetherClass::ViewCamera&,aetherClass::Vector3& rotation,aetherClass::Vector3 lookAtPosition);

	/*
	キーやマウスの処理の読み取り
	*/
	std::pair<aetherClass::Transform,aetherClass::Vector3> mReadKey(const float timeScale);

	void mCheckCameraRotation(aetherClass::Vector3&);
private:
	
	std::shared_ptr<aetherClass::FbxModel> m_model;
	bool m_isHitWall;
	aetherClass::ViewCamera m_playerView;		//　カメラオブジェクト
	aetherClass::Vector3 m_prevPosition;     // 前回のトランスフォーム情報
	aetherClass::Vector3 m_cameraRotation;		//　カメラの回転を管理
	CharaEntity m_charaEntity;					// 便利関数のあるクラスオブジェクト
	Offset m_cameraOffset;						//　カメラのオフセット
	std::shared_ptr<aetherClass::Cube> m_pBodyCollider;   // 基本的なコライダー
	float m_prevRotationY;
	int m_fieldNumber;
};

#endif