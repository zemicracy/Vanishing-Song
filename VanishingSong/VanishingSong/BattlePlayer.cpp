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
void BattlePlayer::mInitialize(eMusical type, aetherClass::Vector3 position, std::shared_ptr<FbxModel> model){
	m_type = type;
	m_model = model;
	m_transform._translation = Vector3(position._x, 0.0f, position._z);
	m_transform._rotation._y = 90;
	m_transform._scale._x = -1;
	return;
}

//
void BattlePlayer::mRender(aetherClass::ShaderBase* shader){
	m_model->KeyframeAnimationRender(shader);
}

//
void BattlePlayer::mUpdate(const float scale){
	// アニメーション系かな？
	m_model->property._transform = m_transform;
	m_model->KeyframeUpdate(m_model->GetKeyframeNameList(0),true);
}

// 自分のタイプを取得
eMusical BattlePlayer::mGetType(){
	return m_type;
}

//
void BattlePlayer::mSetPosition(aetherClass::Vector3 position){
	m_transform._translation = position;
}
