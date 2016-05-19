#pragma once
#include"ActionBoard.h"
#include"GameSound.h"
#include"GameManager.h"
#include"ActionSound.h"
#include"RhythmManager.h"

#include<array>
class OrderList
{
public:

	OrderList();
	~OrderList();

	void mSetFaze(GameManager::eBattleState faze);
	//再生
	void mPlay();


	//EnemyFaze
	void mAddEnemyOrder(std::vector<std::shared_ptr<ActionCommand>>&);	//敵からリストを受け取る


	//PlayerFaze
	void mAddPlayerOrder(std::vector<std::shared_ptr<ActionCommand>>);
	std::shared_ptr<ActionCommand> mGetActionCommand();

	//JudgeFaze
	

	
	void mInitialize(GameManager::eGameMode);
	void mUpdate(float);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);

	//AccesserMethod

private:
	void mPlaySound(std::shared_ptr<ActionSound>);
	void mFinalize();

	//停止
	void mListStop();

	void mRhythmicMotion();

private:

	std::vector<std::shared_ptr<ActionCommand>>m_PlayerOrderList;
	std::vector<std::shared_ptr<ActionCommand>>m_EnemyOrderList;

	std::vector<std::shared_ptr<aetherClass::SpriteBase>>m_pSpriteList;
	std::vector<aetherClass::Vector3>m_pSpriteOrigin;



	std::shared_ptr<ActionBoard>m_ActionCommand;
	std::shared_ptr<aetherClass::SpriteBase>m_pVolumeImage;
	aetherClass::Vector3 m_VolumeOrigin;
	std::shared_ptr<aetherClass::SpriteBase>m_pBackImage;
	aetherClass::Vector3 m_BackImageOrigin;
	aetherClass::Vector3 m_BackImageScaleOrigin;


	RhythmManager *m_rhythm;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTextureList;


	bool m_isStart;			//判定中か
	bool m_isAlPlay;		//既に再生後か？

	bool m_isKeyDown;		//その拍ににキーが押されたか
	bool m_isPlaySound;		//コマンド再生dできるか

	int m_processId;		//処理中のID
	int m_MaxOrderSize;
	
	int m_enemyDamageCounter;
	int m_playerDamageCounter;


	//
	GameManager::eBattleState m_faze;
	GameManager::eGameMode m_mode;
	//定数

	const float m_kMissLevel = 0.5f;
	const float m_kGreat = 0.3f;

};

