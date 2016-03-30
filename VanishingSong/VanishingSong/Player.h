#ifndef _PLAYER_H
#define _PLAYER_H

#include "GearFrame.h"
#include "CharaStatus.h"
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
	void mRender(aetherClass::ShaderBase*);
	void mAction(std::shared_ptr<ActionCommand>);

private:

	/*
		パーツ作成用関数
		第一引数：FBXモデルのパス
		第二引数：パーツの種類
		第三引数：投影するカメラのポインタ
	*/
	std::shared_ptr<Gear> mSetUpGear(std::string path, Gear::eType,aetherClass::ViewCamera*);

	/*
	親子関係を作成するための関数
	第一引数：親のポインタ
	第二引数：子供にするポインタ
	*/
	void mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child);

	/*
		効率的に描画するためのもの
		第一引数：描画するパーツ
		第二引数：シェーダーのポインタ
	*/
	void mGearRender(std::shared_ptr<Gear> gear,aetherClass::ShaderBase*);

	
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
	std::unordered_map<eActionType, int> m_actionHash;
	std::shared_ptr<ActionCommand> m_pAction;
	CharaStatus m_status;
};

#endif