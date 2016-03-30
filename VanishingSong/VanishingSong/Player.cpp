#include "Player.h"
#include "Debug.h"
using namespace aetherClass;
namespace{
	const bool kPlayerDebug = true;
}
Player::Player()
{
	m_pGearFrame = nullptr;
	m_actionHash.clear();
	m_pAction = nullptr;
}


Player::~Player()
{
}


bool Player::mInitialize(ViewCamera* camera){
	m_pGearFrame = std::make_shared<GearFrame>();

	// 体のパーツ
	m_pGearFrame->m_pBody = mSetUpGear("null", Gear::eType::eBody, camera);

	// 腰のパーツ
	m_pGearFrame->m_pWaist = mSetUpGear("null", Gear::eType::eWaist, camera);

	//肩のパーツ
	m_pGearFrame->m_pRightShoulder = mSetUpGear("null", Gear::eType::eRightShourlder, camera);
	m_pGearFrame->m_pLeftShoulder = mSetUpGear("null", Gear::eType::eLeftShourlder, camera);

	// 腕のパーツ
	m_pGearFrame->m_pLeftUpperArm = mSetUpGear("null", Gear::eType::eLeftUpperArm, camera);
	m_pGearFrame->m_pRightUpperArm = mSetUpGear("null", Gear::eType::eRightUpperArm, camera);
	m_pGearFrame->m_pLeftLowerArm = mSetUpGear("null", Gear::eType::eLeftLowerArm, camera);
	m_pGearFrame->m_pRightLowerArm = mSetUpGear("null", Gear::eType::eRightLowerArm, camera);

	// 手のパーツ
	m_pGearFrame->m_pLeftHand = mSetUpGear("null", Gear::eType::eLeftHand, camera);
	m_pGearFrame->m_pRightHand= mSetUpGear("null", Gear::eType::eRightHand, camera);

	// 足のパーツ
	m_pGearFrame->m_pLeftUpperLeg = mSetUpGear("null", Gear::eType::eLeftUpperLeg, camera);
	m_pGearFrame->m_pRightUpperLeg = mSetUpGear("null", Gear::eType::eRightUpperLeg, camera);
	m_pGearFrame->m_pLeftLowerLeg = mSetUpGear("null", Gear::eType::eLeftLowerLeg, camera);
	m_pGearFrame->m_pRightLowerLeg = mSetUpGear("null", Gear::eType::eRightLowerLeg, camera);

	// 体にパーツとの親子関係
	mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
	mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pRightShoulder);
	mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pLeftShoulder);

	// 右腕の親子関係
	mCreateRelationship(m_pGearFrame->m_pRightShoulder, m_pGearFrame->m_pRightUpperArm);
	mCreateRelationship(m_pGearFrame->m_pRightUpperArm, m_pGearFrame->m_pRightLowerArm);
	mCreateRelationship(m_pGearFrame->m_pRightLowerArm,m_pGearFrame->m_pRightHand);

	// 左腕の親子関係
	mCreateRelationship(m_pGearFrame->m_pLeftShoulder, m_pGearFrame->m_pLeftUpperArm);
	mCreateRelationship(m_pGearFrame->m_pLeftUpperArm, m_pGearFrame->m_pLeftLowerArm);
	mCreateRelationship(m_pGearFrame->m_pLeftLowerArm, m_pGearFrame->m_pLeftHand);

	// 右足の親子関係
	mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pRightUpperLeg);
	mCreateRelationship(m_pGearFrame->m_pRightUpperLeg, m_pGearFrame->m_pRightLowerLeg);

	// 左足の親子関係
	mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pLeftUpperLeg);
	mCreateRelationship(m_pGearFrame->m_pLeftUpperLeg, m_pGearFrame->m_pLeftLowerLeg);

	if (kPlayerDebug)
	{
		Debug::mPrint("プレイヤーがデバッグモードです");
	}
	return true;
}

//
void Player::mUpdate(const float timeScale){
	
	return;
}

//
void Player::mRender(aetherClass::ShaderBase* shader){
	
	if (!m_pGearFrame->m_pBody)return;

	// 全ての親は体のパーツなので、必ず体のパーツから始める
	mGearRender(m_pGearFrame->m_pBody, shader);
}

//
void Player::mAction(std::shared_ptr<ActionCommand> command){
	return;
}

/*
	パーツを描画する関数
	引数には描画するパーツを渡す
*/
void Player::mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase* shader){
	// デバッグモードの時はコライダーの表示
	if (kPlayerDebug)
	{
		gear->_pColider->Render(shader);
	}

	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;

	gear->_pGear->Render(shader);

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mGearRender(child, shader);
	}

	return;
}

/*
	パーツひとつひとつの初期化用
	第一：モデルのパス
	第二：初期化するパーツの種類
	第三：カメラのポインタ
*/
std::shared_ptr<Gear> Player::mSetUpGear(std::string path, Gear::eType gearType,aetherClass::ViewCamera* view){
	std::shared_ptr<Gear> pGear;
	bool success;

	// 自分のパーツの初期化
	pGear = std::make_shared<Gear>();
	pGear->_pGear = std::make_shared<FbxModel>();
	// 自身の種類の設定
	pGear->_type = gearType;

	// モデルの読み取り
	success = pGear->_pGear->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!success)
	{
		Debug::mErrorPrint("FBXの初期化が正常に行われませんでした。", __FILE__, __LINE__,Debug::eState::eWindow);

		if (kPlayerDebug)
		{
			// コライダーの初期化
			pGear->_pColider = std::make_shared<Cube>();
			pGear->_pColider->Initialize();
			// コライダーの色を赤に設定
			pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
			pGear->_pColider->SetCamera(view);
			return pGear;
		}
		return nullptr;
	}

	pGear->_pGear->SetCamera(view);
	// テクスチャの読み込み
	//pGear->_pGear->SetTextureDirectoryName("texture")

	// コライダーの初期化
	pGear->_pColider = std::make_shared<Cube>();
	pGear->_pColider->Initialize();
	// コライダーの色を赤に設定
	pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
	pGear->_pColider->SetCamera(view);
	return pGear;
}

/*
	親子関係を作成する関数
	第一引数：親になるポインタ
	第二引数：子供になるポインタ
*/
void Player::mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child){
	parentGear->_pChildren.push_back(child);
	/*	パーツを連結する処理	*/
	child->_pParent = parentGear;
}