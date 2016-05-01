#include "Mode.h"
#include "Debug.h"

Mode::Mode()
{
	m_isInitialize = false;
}


Mode::~Mode()
{
	m_isInitialize = false;
}

bool Mode::mInitialize(GameManager::eDay firstDay){
	// �|�C���^�̏������Ƃ��H
	// �v���C���[�̏�����
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->mInitialize();

	auto view = m_pPlayer->mGetView();
	/*m_penemyGround = std::make_shared<EnemyGround>();
	m_penemyGround->mInitialize(view);*/

	// �X�e�[�W�I�u�W�F�N�g
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	m_pCollideManager = std::make_unique<CollideManager>();
	m_pCollideManager->mInitialize(m_pPlayer, nullptr, m_pFieldArea);

	m_isInitialize = true;
	return true;
}

void Mode::mFinalize(){
	

	if (m_pFieldArea){
		m_pFieldArea.reset();
		m_pFieldArea = nullptr;
	}

	if (m_pPlayer){
		m_pPlayer->mFinalize();
		m_pPlayer.reset();
		m_pPlayer = nullptr;
	}

	/*if (m_penemyGround){
		m_penemyGround->mFinalize();
		m_penemyGround.reset();
		m_penemyGround = nullptr;
	}*/
}

void Mode::mMainUpdate(std::shared_ptr<ActionCommand> command, const float timeScale, const float nowTime){
	if (!m_isInitialize){
		Debug::mPrint("���������I�����Ă��܂���");
		return;
	}
	// �X�V����
	mUpdate(command,timeScale,nowTime);

	// �����蔻��̍X�V
	if (m_pCollideManager){
		m_pCollideManager->mUpdate();
	}

	return;
}

void Mode::mMainRender(ShaderHash shader){
	if (!m_isInitialize){
		Debug::mPrint("���������I�����Ă��܂���");
		return;
	}
	mRender(shader);
	return;
}

// ���ꂼ��̃Q�b�^�[
std::shared_ptr<Player> Mode::mGetPlayer(){
	return m_pPlayer;
}
std::shared_ptr<FieldArea> Mode::mGetFieldArea(){
	return m_pFieldArea;
}
