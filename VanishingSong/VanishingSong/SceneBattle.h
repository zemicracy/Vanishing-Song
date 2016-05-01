//#pragma once

#ifndef _SCENEBATTLE_H
#define _SCENEBATTLE_H
#include<GameScene.h>
#include<ModelBase.h>
#include<ShaderBase.h>
#include<memory>
#include<ViewCamera.h>
#include<Texture.h>

class SceneBattle :
	public aetherClass::GameScene
{
private:
	std::unique_ptr<aetherClass::ModelBase> m_pModelBase;
	std::unique_ptr<aetherClass::ShaderBase> m_pShaderBase;
	std::unique_ptr<aetherClass::ViewCamera> m_pTitleView;
	std::unique_ptr<aetherClass::Texture>m_pTexture;
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

public:
	static const std::string Name;
};


#endif