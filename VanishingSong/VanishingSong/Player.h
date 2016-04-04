#ifndef _PLAYER_H
#define _PLAYER_H

#include "GearFrame.h"
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"
#include <ShaderBase.h>
#include <unordered_map>
class ActionCommand;
class Player
{
public:
	Player();
	~Player();
	bool mInitialize(aetherClass::ViewCamera*);
	void mUpdate(const float timeScale);
	//void mUpdate(const float timeScale,std::shared_ptr<ActionCommand>);
	void mRender(aetherClass::ShaderBase* modelShader,aetherClass::ShaderBase* colliderShader);
	void mAction(std::shared_ptr<ActionCommand>);

private:

	

	/*
		プレイヤーに対するキー入力処理
		現状移動処理のみ
	*/
	aetherClass::Vector3 mReadKey(const float timeScale);
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
	std::unordered_map<eActionType, int> m_actionHash;
	std::shared_ptr<ActionCommand> m_pActionCommand;
	std::shared_ptr<Gear> m_pTopGear;
	CharaStatus m_status;
	CharaEntity m_charaEntity;
};

#endif