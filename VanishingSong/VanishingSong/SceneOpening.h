//#pragma once
#ifndef _SCENEGAME_H
#define _SCENEGAME_H

#include<GameScene.h>
#include<ShaderBase.h>
#include<SpriteBase.h>
#include<Texture.h>
#include<memory>
#include<GameClock.h>
#include"GameSound.h"

class SceneOpening :
	public aetherClass::GameScene
{
public:
	static const std::string Name;
private:
	std::unique_ptr<aetherClass::SpriteBase>m_pSpriteBase;

	//BGM用
	std::shared_ptr<aetherClass::GameSound> m_openingSound;

	//テクスチャ用の配列
	std::unique_ptr<aetherClass::Texture>m_pTexture[17];

	//時間カウント用変数
	double m_clockCount;

	//配列番号の切り換え用変数
	int m_array;

	//テクスチャ表示繰り返し用変数
	int m_imageCount;
	std::shared_ptr<aetherClass::SpriteBase> m_pSkip;
	aetherClass::Texture m_skipTexture;

	//テクスチャ繰り返し表示と続けて表示するパターン、切り替え用の変数
	int m_openingflg;

	bool m_isInit;
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