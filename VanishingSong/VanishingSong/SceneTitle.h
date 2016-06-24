
#ifndef _SCENETITLE_H
#define _SCENETITLE_H
#include <memory>
#include <array>
#include <GameScene.h>
#include <ShaderBase.h>
#include <ViewCamera.h>
#include <Texture.h>
#include <SpriteBase.h>
#include <Skybox.h>
#include "GameManager.h"
#include"GameSound.h"
#include "FieldArea.h"
#include "Config.h"
class SceneTitle :
	public aetherClass::GameScene
{
	// 列挙型定数
	enum eNextMode{
		eNull = 0,
		eStart,
		eLoad,
		eCredit,
		eExit
	};

	// カーソル用
	struct ModeSelect{
		ModeSelect(){

		}

		ModeSelect(const int null){
			_cursorY = 0.0f;
			_modeNumber = null;
		}
		float _cursorY;
		int _modeNumber;
	};

	// シーン遷移用
	struct SceneInfo{
		std::string _nextSceneName;
	};

	struct  TitlePlayer
	{
		std::shared_ptr<aetherClass::FbxModel> _model;
		std::string _animationName;
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
	SceneInfo mGetGameMode(const int index);
	void mChangeSelect(const bool isUp, const bool isDown);
	void mCursorState(const bool isStart);
	bool mMenuSelectState(const bool isReturn, const  std::pair<bool, bool>);
	void mSetPlayer(eMusical);
private:
	std::shared_ptr<aetherClass::SpriteBase> m_pLogo;
	std::shared_ptr<aetherClass::SpriteBase> m_pMenu;
	std::shared_ptr<aetherClass::SpriteBase> m_pCursor;
	std::shared_ptr<aetherClass::Texture> m_pLogoTexture;
	std::shared_ptr<aetherClass::Texture> m_pMenuTexture;
	std::shared_ptr<aetherClass::Texture> m_pPushTexture;
	
	std::unordered_map<eMusical, TitlePlayer> m_players;
	std::shared_ptr<RhythmManager> m_pRhythmManager;
	std::shared_ptr<aetherClass::GameSound> m_pBGM;
	std::shared_ptr<aetherClass::GameSound> m_returnSE;
	std::shared_ptr<aetherClass::GameSound> m_selectSE;
	std::array<ModeSelect, 4> m_cursorArray;
	std::shared_ptr<FieldArea>  m_pField;
	FieldNote m_fieldNote;

	bool m_pushState;
	int m_nowSelectMode;
	bool m_alphaState;
	int m_nowCursor;
	bool m_isVisibleSaveData;

	aetherClass::ViewCamera m_view;

	Config m_config;
};

#endif