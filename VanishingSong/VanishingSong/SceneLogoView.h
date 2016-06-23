#pragma once

#include<Rectangle2D.h>
#include<GameScene.h>
#include<GameSound.h>

class SceneLogoView : public aetherClass::GameScene
{
public:
	static const std::string Name;

public:
	SceneLogoView();
	~SceneLogoView();

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
	bool mChangeTexture(int);
	float mFrameTime;

	std::unordered_map<int,std::shared_ptr<aetherClass::Texture>>m_pTextureList;
	std::shared_ptr<aetherClass::SpriteBase>m_pSprite;
	std::shared_ptr<aetherClass::GameSound>m_pSoundDevice;

};

