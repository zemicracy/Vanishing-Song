//#pragma once

#ifndef _SCENEBATTLE_H
#define _SCENEBATTLE_H
#include <GameScene.h>
#include <ModelBase.h>
#include <ShaderBase.h>
#include <memory>
#include <ViewCamera.h>
#include <Texture.h>
#include "GameManager.h"
#include"OrderList.h"
class SceneBattle :
	public aetherClass::GameScene
{

public:
	static const std::string Name;
public:
	SceneBattle();
	~SceneBattle();

	//初期化
	bool Initialize()override;
	void Finalize()override;

	//1フレームごとの更新処理
	bool Updater()override;

	//1フレームごとの描画処理
	void Render()override;

	//1フレームごとのui描画処理
	void UIRender()override;

	//トランジション効果を設定
	bool TransitionIn()override;
	bool TransitionOut()override;

private:
	void OnListen();    // 敵の演奏
	void OnPerform();   // プレイヤーの演奏
	void OnBattle();    // 戦闘開始
	void CheckBattle();
private:
	GameManager::eBattleState m_battleState;
	std::unique_ptr<aetherClass::ModelBase> m_pModelBase;
	std::unique_ptr<aetherClass::Texture>m_pTexture;

	std::unique_ptr<OrderList> m_pOrderList;


	aetherClass::ViewCamera m_view;
};


#endif