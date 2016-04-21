//#pragma once
#ifndef _SCENEGAMEOVER_H
#define _SCENEGAMEOVER_H
#include<GameScene.h>
#include<ModelBase.h>
#include<ShaderBase.h>
#include<memory>
#include<ViewCamera.h>
#include<SpriteBase.h>
#include<Texture.h>
class SceneGameover:
	public aetherClass::GameScene
{
private:
	std::unique_ptr<aetherClass::ModelBase> m_pModelBase;
	std::unique_ptr<aetherClass::ShaderBase> m_pShaderBase;
	std::unique_ptr<aetherClass::ViewCamera> m_pTitleView;
	std::unique_ptr<aetherClass::SpriteBase>m_pSpriteBase;
	std::unique_ptr<aetherClass::Texture>m_pTexture;
public:
	SceneGameover();
	~SceneGameover();

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