#include "Player.h"
#include "Debug.h"
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
using namespace aetherClass;

Player::Player()
{
	m_pGearFrame = nullptr;
	m_actionHash.clear();
	m_pActionCommand = nullptr;
	
}


Player::~Player()
{
}


bool Player::mInitialize(ViewCamera* camera){
	m_pGearFrame = std::make_shared<GearFrame>();

	// 体のパーツ
	m_pGearFrame->m_pBody = m_charaEntity.mSetUpGear("null", Gear::eType::eBody, camera);

	// 腰のパーツ
	m_pGearFrame->m_pWaist = m_charaEntity.mSetUpGear("null", Gear::eType::eWaist, camera);

	//肩のパーツ
	m_pGearFrame->m_pRightShoulder = m_charaEntity.mSetUpGear("null", Gear::eType::eRightShourlder, camera);
	m_pGearFrame->m_pLeftShoulder = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftShourlder, camera);

	// 腕のパーツ
	m_pGearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperArm, camera);
	m_pGearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperArm, camera);
	m_pGearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerArm, camera);
	m_pGearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerArm, camera);

	// 手のパーツ
	m_pGearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftHand, camera);
	m_pGearFrame->m_pRightHand = m_charaEntity.mSetUpGear("null", Gear::eType::eRightHand, camera);

	// 足のパーツ
	m_pGearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftUpperLeg, camera);
	m_pGearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightUpperLeg, camera);
	m_pGearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eLeftLowerLeg, camera);
	m_pGearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("null", Gear::eType::eRightLowerLeg, camera);

	// 体にパーツとの親子関係
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pRightShoulder);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pLeftShoulder);

	// 右腕の親子関係
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightShoulder, m_pGearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightUpperArm, m_pGearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightLowerArm, m_pGearFrame->m_pRightHand);

	// 左腕の親子関係
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftShoulder, m_pGearFrame->m_pLeftUpperArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftUpperArm, m_pGearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftLowerArm, m_pGearFrame->m_pLeftHand);

	// 右足の親子関係
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pRightUpperLeg, m_pGearFrame->m_pRightLowerLeg);

	// 左足の親子関係
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(m_pGearFrame->m_pLeftUpperLeg, m_pGearFrame->m_pLeftLowerLeg);

	WorldReader read;
	read.Load("data\\Player.aether");
	for (auto index : read.GetInputWorldInfo()._object)
	{
		if (index->_name == "Body")
		{
			m_pGearFrame->m_pBody->_pColider->property._transform = index->_transform;
		}

		if (index->_name == "LeftArm")
		{
			m_pGearFrame->m_pLeftUpperArm->_pColider->property._transform = index->_transform;
		}

		if (index->_name == "LeftLowerArm")
		{
			m_pGearFrame->m_pLeftLowerArm->_pColider->property._transform = index->_transform;
		}
	}
	
	read.UnLoad();
	if (kCharaDebug)
	{
		Debug::mPrint("プレイヤーがデバッグモードです");
	}
	return true;
}

//
void Player::mUpdate(const float timeScale){
	
	// 移動処理
	mReadKey(timeScale);

	return;
}

//
void Player::mReadKey(const float timeScale){
	Vector3 move = kVector3Zero;

	// 奥行の移動(Z軸)
	if (GameController::GetKey().IsKeyDown('W')){
		move._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		move._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// 横の移動(X軸)
	if (GameController::GetKey().IsKeyDown('D')){
		move._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		move._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	m_pGearFrame->m_pBody->_pColider->property._transform._translation += move;
	Vector3 hoge = m_pGearFrame->m_pBody->_pColider->property._transform._translation;
	
	if (kCharaDebug&&GameController::GetKey().KeyDownTrigger('P')){
		Debug::mPrint("---Playerの現在の座標---");
		Debug::mPrint("X :" + std::to_string(hoge._x));
		Debug::mPrint("Y :" + std::to_string(hoge._y));
		Debug::mPrint("Z :" + std::to_string(hoge._z));
		Debug::mPrint("------------------------");
	}
	m_pGearFrame->m_pBody->_pColider->property._transform._translation += move;
}


//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){
	
	if (!m_pGearFrame->m_pBody)return;

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	m_charaEntity.mGearRender(m_pGearFrame->m_pBody, modelShader,colliderShader);
}

//
void Player::mAction(std::shared_ptr<ActionCommand> command){
	return;
}
