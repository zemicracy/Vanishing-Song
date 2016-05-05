/*
	ゲームモードの基底クラス
*/

#ifndef _MODE_H
#define _MODE_H
#include <memory>
#include "ActionCommand.h"
#include "GameManager.h"
#include "EnemyManager.h"
#include "Player.h"
#include "FieldArea.h"
#include "OrderList.h"
#include "ActionBoard.h"
#include "CollideManager.h"
#include "GaugeManager.h"
typedef std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>> ShaderHash;

class Mode
{
public:
	enum class eState{
		eClear,
		eGameOver,
		eNextDay,
		eNull
	};
public:
	Mode();
	~Mode();

	virtual bool mInitialize(GameManager::eSkillType skill, GameManager::eDay firstDay);
	void mMainUpdate(const float timeScale, const float nowTime);
	void mMainRender(ShaderHash shader);
	void mMainUIRender(ShaderHash shader);
	// 次の日に進むときの初期化処理
	virtual void mNextDayInitialize(GameManager::eDay nextDay);

	// 前の日の解放処理
	virtual void mPrevDayFinalize(GameManager::eDay prevDay);

	virtual void mFinalize();

	eState mGetState();
	ResultData mGetResultData();
protected:
	std::shared_ptr<Player> mGetPlayer();
	std::shared_ptr<FieldArea> mGetFieldArea();
	void mSetState(eState);
private:
	virtual void mUpdate(const float timeScale, const float nowTime) = 0;
	virtual void mRender(ShaderHash shader) = 0;
	virtual void mUIRender(ShaderHash shader) = 0;
private:

	//std::shared_ptr<EnemyManager> m_penemyGround;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<EnemyManager> m_pEnemyManager;
	std::shared_ptr<FieldArea> m_pFieldArea;
	std::unique_ptr<GaugeManager> m_pPlayerGaugeManager;
	std::unique_ptr<CollideManager> m_pCollideManager;
	std::unique_ptr<OrderList> m_pOrderList;
	std::unique_ptr<ActionBoard> m_pActionBoard;
	bool m_isInitialize;
	eState m_state;
};

#endif