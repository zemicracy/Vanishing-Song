#include "BattlePlayer.h"

using namespace aetherClass;
namespace{
	const int kWaitAnimation = 60;
	const int kZero = 0;
}
BattlePlayer::BattlePlayer()
{
}


BattlePlayer::~BattlePlayer()
{
}

//
void BattlePlayer::mInitialize(eMusical type, aetherClass::Vector3 position, std::shared_ptr<GearFrame> gearframe){
	m_type = type;
	m_playerGear = gearframe;
	m_transform._translation = position;
	m_transform._translation._y = 22.2f;
	m_transform._rotation._y = 90;

	//
	// 連想配列に登録
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eBody, m_playerGear->m_pBody);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eWaist, m_playerGear->m_pWaist);

	// 左
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eLeftHand, m_playerGear->m_pLeftHand);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eLeftLowerArm, m_playerGear->m_pLeftLowerArm);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eLeftLowerLeg, m_playerGear->m_pLeftLowerLeg);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eLeftUpperArm, m_playerGear->m_pLeftUpperArm);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eLeftUpperLeg, m_playerGear->m_pLeftUpperLeg);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eLeftFoot, m_playerGear->m_pLeftFoot);
	// 右
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eRightHand, m_playerGear->m_pRightHand);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eRightLowerArm, m_playerGear->m_pRightLowerArm);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eRightLowerLeg, m_playerGear->m_pRightLowerLeg);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eRightUpperArm, m_playerGear->m_pRightUpperArm);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eRightUpperLeg, m_playerGear->m_pRightUpperLeg);
	m_charaEntity.mRegisterParts(m_gearHash, Gear::eType::eRightFoot, m_playerGear->m_pRightFoot);
	mRegisterAnimation(eState::eWait, kWaitAnimation, "data\\Player\\Stay.aether", "data\\Player\\Wait.aether");

	return;
}

//
void BattlePlayer::mRender(aetherClass::ShaderBase* shader){
	m_charaEntity.mGearRender(m_playerGear->m_pBody, shader, shader);
}

//
void BattlePlayer::mUpdate(const float scale){
	// アニメーション系かな？
	mAnimation(eState::eWait);
	m_charaEntity.mGearMove(m_playerGear->m_pBody, m_transform._translation);
	m_charaEntity.mGearRotation(m_playerGear->m_pBody, m_playerGear->m_pBody, m_transform._rotation);

}

// 自分のタイプを取得
eMusical BattlePlayer::mGetType(){
	return m_type;
}

//
void BattlePlayer::mSetPosition(aetherClass::Vector3 position){
	m_transform._translation = position;
}

//
void BattlePlayer::mAnimation(eState state){
	// 前回と違うときは更新
	if (m_prevState != state){
		m_counter.first = NULL;
		m_prevState = state;
	}

	// 設定されていない場合何もしない
	if (m_animationHash.find(state) == m_animationHash.end()) return;

	Transform animationTransform;
	/*	アニメーション実行処理	*/
	const int allFrame = m_animationHash[state]._animationFrame;

	for (auto index : m_animationHash[state]._animation)
	{
		// 補間の値を取得
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end, allFrame, m_counter.first);
		// 反転用、体だけ反転させる
		if (index._name == Gear::eType::eBody){
			animationTransform._scale._x = -1;
		}
		// アニメーションの適用
		if (m_gearHash.find(index._name) != m_gearHash.end()){
			m_gearHash[index._name]->_pGear->property._transform = animationTransform;
		}
	}

	switch (state)
	{
	case eState::eWait:
		/*	カウンターの状態を切り替える	*/
		if (m_counter.first > kWaitAnimation){
			m_counter.second = true;
		}
		else if (m_counter.first < kZero){
			m_counter.second = false;
		}

		// カウンターの状態に合わせてフレームカウントの更新
		if (m_counter.second){
			m_counter.first -= 1;
		}
		else{
			m_counter.first += 1;
		}
		break;
	default:
		break;
	}

	return;
}

//
void BattlePlayer::mRegisterAnimation(eState key, const int allFrame, std::string first, std::string last){
	AnimationFrame animation;
	bool result = false;

	// 既にある登録済みなら何もしない
	if (m_animationHash.find(key) != m_animationHash.end()) return;

	result = m_charaEntity.mLoadAnimation(animation._animation, first, last);
	if (!result)
	{
		Debug::mErrorPrint("読み込み失敗", __FILE__, __LINE__);
		return;
	}
	animation._animationFrame = allFrame;

	// 登録
	m_animationHash.insert(std::make_pair(key, animation));
	return;
}