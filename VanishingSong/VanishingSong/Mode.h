/*
	�Q�[�����[�h�̊��N���X
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
typedef std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>> ShaderHash;
class Mode
{
public:
	Mode();
	~Mode();

	virtual bool mInitialize(GameManager::eDay firstDay);
	void mMainUpdate(std::shared_ptr<ActionCommand> command, const float timeScale, const float nowTime);
	void mMainRender(ShaderHash shader);

	// ���̓��ɐi�ނƂ��̏���������
	virtual void mNextDayInitialize(GameManager::eDay nextDay) = 0;

	// �O�̓��̉������
	virtual void mPrevDayFinalize(GameManager::eDay prevDay) = 0;

	virtual void mFinalize();

protected:
	std::shared_ptr<Player> mGetPlayer();
	std::shared_ptr<FieldArea> mGetFieldArea();

private:
	virtual void mUpdate(std::shared_ptr<ActionCommand> command, const float timeScale, const float nowTime) = 0;
	virtual void mRender(ShaderHash shader) = 0;
private:

	//std::shared_ptr<EnemyManager> m_penemyGround;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<FieldArea> m_pFieldArea;

	std::unique_ptr<CollideManager> m_pCollideManager;

	bool m_isInitialize;
};

#endif