#ifndef _FieldEnemyManager_H
#define _FieldEnemyManager_H
#include <map>
#include "FieldEnemy.h"
#include "GameClock.h"
#include "MessageWindow.h"


class FieldEnemyManager
{

public:
	FieldEnemyManager();
	~FieldEnemyManager();

	bool mInitilize(aetherClass::ViewCamera*);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdater();
	void mFinalize();
	void mSetPosion();
	bool mInitilizeMessage();
	void mRenderMessage(bool);
	void mmVisibleMessage();
	bool mGetIsJudge();
	std::shared_ptr<FieldEnemy> mEnemyGet(int);

private:

	std::vector<std::shared_ptr<FieldEnemy>> m_pEnemy;
	std::array<std::shared_ptr<FieldEnemy>,4> m_enemyArray;
	std::array<aetherClass::Vector3, 4>	m_pEnemySpawner;	//oŒ»êŠ
	std::shared_ptr<MessageWindow> m_pEnemyMessage;
	std::shared_ptr<aetherClass::Texture> m_pMessageTexture;
	
	int EnemySize;	//Enemy”
	int EnemyMaxSize;
	bool m_isRender;
	bool m_isJudge;
};
#endif