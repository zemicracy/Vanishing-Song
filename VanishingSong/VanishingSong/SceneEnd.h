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
#include <vector>
#include "MessageWindow.h"

class SceneEnd:
	public aetherClass::GameScene
{
private:
	struct EndActor
	{

		EndActor(){
			_animationCount = NULL;
			_animationName = "null";
			_prevAnimationName = "null";
		}

		~EndActor(){
			if (_model){
				_model.reset();
				_model = nullptr;
			}
			_animationCount = NULL;
			_animationName = "null";
			_prevAnimationName = "null";
		}
		std::shared_ptr<aetherClass::FbxModel> _model;
		std::string _animationName;
		std::string _prevAnimationName;
		int _animationCount;
	};

	struct Message
	{
		Message(){
			_count = NULL;
		}
		std::shared_ptr<aetherClass::Texture> _text;
		std::shared_ptr<aetherClass::Texture> _icon;
		int _count;
	};
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
	void mActorAnimation(std::unordered_map<eMusical, EndActor>&,EndActor& boss);
	void mChangeMessage(MessageWindow&,Message&, const int);
private:
	aetherClass::GameSound m_bgm;
	std::shared_ptr<aetherClass::SpriteBase> m_pEndBord;
	aetherClass::Texture m_endBordTexture;
	
	std::unordered_map<eMusical, EndActor> m_actors;
	EndActor m_boss;
	std::unique_ptr<aetherClass::Skybox> m_pSkybox;
	FieldArea m_field;
	aetherClass::ViewCamera m_view;
	std::vector<std::string> m_textPathList;
	std::vector<std::string> m_iconPathList;
	aetherClass::Texture m_button;
	MessageWindow m_messageWindow;
	Message m_message;
	bool m_isMessageEnd;
	bool m_isTransitionEnd;
	aetherClass::GameSound m_returnSE;
	bool m_isFade;
	bool m_isFade2;
	bool m_isRender;
	float m_backToTitleTime;
};

#endif