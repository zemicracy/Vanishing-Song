#include "BattlePlayer.h"

using namespace aetherClass;
namespace{
	const int kWaitAnimation = 60;
	const int kMaxAttackFrame = 30;
	const int kZero = 0;
	const std::string kDefaultAnimationName = "attackwait";
	const std::string kWinAnimationName = "win";
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
	m_transform._scale = Vector3(-1, 1, 1);
	m_animationFrame = NULL;
	return;
}

//
void BattlePlayer::mRender(aetherClass::ShaderBase* shader){
	m_model->KeyframeAnimationRender(shader);
}

//
void BattlePlayer::mUpdate(const float scale){
	// �A�j���[�V�����n���ȁH
	++m_animationFrame;
	if (m_animationName == kWinAnimationName){
		if (m_animationFrame >= kMaxAttackFrame-1){
			m_animationFrame = 0;
		}
	}
	

	if (m_animationName == kDefaultAnimationName){
		if (m_animationFrame >= kWaitAnimation - 1){
			m_animationName = kDefaultAnimationName;
			m_animationFrame = 0;
		}
	}
	else{
		if (m_animationFrame >= kMaxAttackFrame - 1){
			m_animationName = kDefaultAnimationName;
			m_animationFrame = 0;
		}
	}

	m_model->KeyframeUpdate(m_animationName, m_animationFrame);
	m_model->property._transform = m_transform;
}

// �����̃^�C�v���擾
eMusical BattlePlayer::mGetType(){
	return m_type;
}

//
void BattlePlayer::mSetPosition(aetherClass::Vector3 position){
	m_transform._translation = position;
}


void BattlePlayer::mChangeAnimation(std::string name){
	m_animationFrame = 0;
	m_animationName = name;
}
