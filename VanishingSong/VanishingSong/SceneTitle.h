
#ifndef _SCENETITLE_H
#define _SCENETITLE_H
#include <memory>
#include <array>
#include <GameScene.h>
#include <ShaderBase.h>
#include <ViewCamera.h>
#include <Texture.h>
#include <SpriteBase.h>
#include "GameManager.h"
class SceneTitle :
	public aetherClass::GameScene
{
	// 列挙型定数
	enum eNextMode{
		eNull = 0,
		eSurvival,
		eBattle,
		ePractice,
		eTutorial,
		eExit
	};
	struct ModeSelect{
		float _cursorY;
		int _modeNumber;
	};

public:
	SceneTitle();

	~SceneTitle();

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
private:
	std::string mGetGameMode(const int index);
	void mChangeSelect(aetherClass::Vector2);
	void mClickState();
	bool mMenuSelectState();
private:
	std::unique_ptr<aetherClass::SpriteBase> m_pLogo;
	std::shared_ptr<aetherClass::ShaderBase> m_pTextureShader;
	std::shared_ptr<aetherClass::ShaderBase> m_pColorShader;
	std::shared_ptr<aetherClass::Texture> m_pLogoTexture;
	std::shared_ptr<aetherClass::Texture> m_pMenuTexture;
	std::shared_ptr<aetherClass::Texture> m_pPushTexture;
	std::unique_ptr<aetherClass::SpriteBase> m_pMenu;
	std::unique_ptr<aetherClass::SpriteBase> m_pCursor;
	std::array<ModeSelect, 5> m_cursorArray;
	bool m_pushState;
	int m_nowSelectMode;
	bool m_alphaState;
};

#endif