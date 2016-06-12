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
	m_transform._translation = position;
	m_transform._rotation._y = 90;

	
	return;
}

//
void BattlePlayer::mRender(aetherClass::ShaderBase* shader){
	m_model->Render(shader);
}

//
void BattlePlayer::mUpdate(const float scale){
	// �A�j���[�V�����n���ȁH
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
