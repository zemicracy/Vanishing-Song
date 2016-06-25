//#pragma once
#ifndef _SCENEGAME_H
#define _SCENEGAME_H

#include<GameScene.h>
#include<ShaderBase.h>
#include<SpriteBase.h>
#include<Texture.h>
#include<memory>
#include<GameClock.h>

class SceneOpening :
	public aetherClass::GameScene
{
private:
	std::unique_ptr<aetherClass::ShaderBase> m_pShaderBase;
	std::unique_ptr<aetherClass::SpriteBase>m_pSpriteBase;

	//テクスチャ用の配列
	std::unique_ptr<aetherClass::Texture>m_pTexture[16];

	double m_clockCount;
	int m_array;
	int m_imageCount;
public:
	SceneOpening();
	~SceneOpening();

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
};

#endif