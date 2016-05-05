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

bool Mode::mInitialize(GameManager::eSkillType skill, GameManager::eDay firstDay){

	// �A�N�V�����R�}���h�̏�����
	m_pActionBoard = std::make_unique<ActionBoard>();
	m_pActionBoard->mInitialize(skill);

	// �v���C���[�̏�����
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->mInitialize();

	auto view = m_pPlayer->mGetView();
	m_pEnemyManager = std::make_shared<EnemyManager>();
	m_pEnemyManager->mInitilize(view);

	// �X�e�[�W�I�u�W�F�N�g
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	m_pCollideManager = std::make_unique<CollideManager>();
	m_pCollideManager->mInitialize(m_pPlayer, m_pEnemyManager, m_pFieldArea);

	m_pPlayerGaugeManager = std::make_unique<GaugeManager>();
	m_pPlayerGaugeManager->mInitialize();	
	m_pPlayerGaugeManager->mSetCharaStatus(&m_pPlayer->mGetStatus());

	// �I�[�_�[���X�g�̏�����
	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize();
	m_pOrderList->mSetCharaMp(&m_pPlayer->mGetStatus()._mp);

	m_state = eState::eNull;
	m_isInitialize = true;
	return true;
}

void Mode::mFinalize(){
	
	if (m_pPlayer){
		m_pPlayer.reset();
		m_pPlayer = nullptr;
	}

	if (m_pPlayerGaugeManager){
		m_pPlayerGaugeManager.release();
		m_pPlayerGaugeManager = nullptr;
	}

	if (m_pOrderList){
		m_pOrderList.release();
		m_pOrderList = nullptr;
	}

	if (m_pActionBoard){

		m_pActionBoard.release();
		m_pActionBoard = nullptr;
	}

	if (m_pFieldArea){
		m_pFieldArea.reset();
		m_pFieldArea = nullptr;
	}

	if (m_pCollideManager){
		m_pCollideManager.release();
		m_pCollideManager = nullptr;
	}

	if (m_pEnemyManager){
		m_pEnemyManager->mFinalize();
		m_pEnemyManager.reset();
		m_pEnemyManager = nullptr;
	}
}

void Mode::mMainUpdate(const float timeScale, const float nowTime){
	if (!m_isInitialize){
		Debug::mPrint("���������I�����Ă��܂���");
		return;
	}
	
	m_pActionBoard->mUpdate(timeScale);

	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	m_pActionBoard->mUpdate(timeScale);
	m_pOrderList->mUpdate(timeScale);

	mGetPlayer()->mUpdate(timeScale, m_pOrderList->mGetActionCommand());

	// �h����̍X�V����
	mUpdate(timeScale,nowTime);

	m_pPlayerGaugeManager->mSetuseMp(m_pOrderList->mGetIfUseMp());
	m_pPlayerGaugeManager->mUpdate(timeScale);

	m_pEnemyManager->mUpdater();

	// �����蔻��̍X�V
	m_pCollideManager->mUpdate();

	// �v���C���[�̎��S����
	bool playerDead = mGetPlayer()->mIsDead();
	if (playerDead){
		mSetState(eState::eGameOver);
	}

	return;
}

void Mode::mMainRender(ShaderHash shaderHash){
	if (!m_isInitialize){
		Debug::mPrint("���������I�����Ă��܂���");
		return;
	}
	
	mRender(shaderHash);
	mGetPlayer()->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	mGetFieldArea()->mRender(shaderHash["color"].get(), shaderHash["texture"].get());
	m_pEnemyManager->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	return;
}

void Mode::mMainUIRender(ShaderHash shader){
	if (!m_isInitialize){
		Debug::mPrint("���������I�����Ă��܂���");
		return;
	}
	
	m_pActionBoard->mRender(shader["color"].get());
	m_pOrderList->mRender(shader["texture"].get(), shader["color"].get());
	m_pPlayerGaugeManager->mRender();
	mUIRender(shader);
	return;
}

// ���ꂼ��̃Q�b�^�[
std::shared_ptr<Player> Mode::mGetPlayer(){
	return m_pPlayer;
}
std::shared_ptr<FieldArea> Mode::mGetFieldArea(){
	return m_pFieldArea;
}


Mode::eState Mode::mGetState(){
	return m_state;
}

void  Mode::mSetState(Mode::eState state){
	m_state = state;
}


ResultData Mode::mGetResultData(){
	return mGetPlayer()->mGetResultData();
}

void Mode::mNextDayInitialize(GameManager::eDay nextDay){
	mGetPlayer()->mDayReset();
}

// �O�̓��̉������
void Mode::mPrevDayFinalize(GameManager::eDay prevDay){

}