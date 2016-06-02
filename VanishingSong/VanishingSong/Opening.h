//#pragma once
#ifndef _SCENEGAME_H
#define _SCENEGAME_H

#include<GameScene.h>
#include<ShaderBase.h>
#include<ViewCamera.h>
#include<SpriteBase.h>
#include<Texture.h>
#include<memory>
#include<GameClock.h>

//#include<OpeningPicture.h>

class Opening:
	public aetherClass::GameScene
{
private:
	std::unique_ptr<aetherClass::ShaderBase> m_pShaderBase;
	std::unique_ptr<aetherClass::ViewCamera> m_pOpeningView;
	//std::unique_ptr<aetherClass::SpriteBase>m_pSpriteBase;
	std::unique_ptr<aetherClass::ModelBase>m_pModelBase;
	std::unique_ptr<aetherClass::Texture>m_pTexture[8];
	double m_clockCount;
	int m_array;
	int m_imageCount;
public:
	Opening();
	~Opening();

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