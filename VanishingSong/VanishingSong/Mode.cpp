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

	// アクションコマンドの初期化
	m_pActionBoard = std::make_unique<ActionBoard>();
	m_pActionBoard->mInitialize(skill);

	// オーダーリストの初期化
	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize();

	// プレイヤーの初期化
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->mInitialize();

	auto view = m_pPlayer->mGetView();
	/*m_penemyGround = std::make_shared<EnemyGround>();
	m_penemyGround->mInitialize(view);*/

	// ステージオブジェクト
	m_pFieldArea = std::make_shared<FieldArea>();
	m_pFieldArea->mInitialize();
	m_pFieldArea->mSetCamera(view);

	m_pCollideManager = std::make_unique<CollideManager>();
	m_pCollideManager->mInitialize(m_pPlayer, nullptr, m_pFieldArea);

	m_pPlayerGaugeManager = std::make_unique<GaugeManager>();
	m_pPlayerGaugeManager->mInitialize();
	m_pPlayerGaugeManager->mSetCharaStatus(&m_pPlayer->mGetStatus());

	m_isInitialize = true;
	return true;
}

void Mode::mFinalize(){
	
	if (m_pPlayer){
		m_pPlayer->mFinalize();
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

	/*if (m_penemyGround){
		m_penemyGround->mFinalize();
		m_penemyGround.reset();
		m_penemyGround = nullptr;
	}*/
}

void Mode::mMainUpdate(const float timeScale, const float nowTime){
	if (!m_isInitialize){
		Debug::mPrint("初期化が終了していません");
		return;
	}

	m_pActionBoard->mUpdate(timeScale);

	auto actionCommand = m_pActionBoard->mSelectType();
	if (actionCommand){
		m_pOrderList->mAddOrder(actionCommand);
	}

	m_pActionBoard->mUpdate(timeScale);
	m_pOrderList->mUpdate(timeScale);

	// 更新処理
	mUpdate(m_pOrderList->mGetActionCommand(),timeScale,nowTime);

	m_pPlayerGaugeManager->mUpdate(timeScale);
	// 当たり判定の更新
	if (m_pCollideManager){
		m_pCollideManager->mUpdate();
	}

	return;
}

void Mode::mMainRender(ShaderHash shader){
	if (!m_isInitialize){
		Debug::mPrint("初期化が終了していません");
		return;
	}
	
	mRender(shader);
	return;
}

void Mode::mMainUIRender(ShaderHash shader){
	if (!m_isInitialize){
		Debug::mPrint("初期化が終了していません");
		return;
	}
	return;
	m_pActionBoard->mRender(shader["color"].get());
	m_pOrderList->mRender(shader["color"].get());
	m_pPlayerGaugeManager->mRender();
	mUIRender(shader);
	return;
}

// それぞれのゲッター
std::shared_ptr<Player> Mode::mGetPlayer(){
	return m_pPlayer;
}
std::shared_ptr<FieldArea> Mode::mGetFieldArea(){
	return m_pFieldArea;
}
