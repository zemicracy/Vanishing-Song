#ifndef _SCENEEND_H
#define _SCENEEND_H

#include <memory>
#include <array>
#include <GameScene.h>
#include <ViewCamera.h>
#include <Texture.h>
#include <Skybox.h>
#include "GameManager.h"
#include"GameSound.h"
#include "FieldArea.h"

class SceneEnd:
	public aetherClass::GameScene
{

public:
	static const std::string Name;

public:
	SceneEnd();
	~SceneEnd();

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
	std::shared_ptr<aetherClass::GameSound> m_bgm;
	std::array<std::shared_ptr<aetherClass::FbxModel>,4> m_pPlayer;
	std::shared_ptr<aetherClass::FbxModel> m_pBoss;
	std::unique_ptr<aetherClass::Skybox> m_pSkybox;
	FieldArea m_field;
	aetherClass::ViewCamera m_view;

};

#endif