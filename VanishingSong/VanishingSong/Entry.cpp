#ifndef _AETHER_H
#define _AETHER_H
#pragma comment (lib,"Aether.lib")
#endif
#include <memory>
#include <AetherWindow.h>
#include <GameController.h>
#include <ConsoleWindow.h>
#include <GameSceneManager.h>
#include "Const.h"
#include "VanishingSongFrame.h"
#include "Load.h"
#include"SceneLogoView.h"
#include "SceneTitle.h"
using namespace aetherClass;

namespace{
	const int kError = -1;
	const int kEnd = 0;
	const int kWindowNum = 1;
}

// エントリーポイントを作成
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT){
	
	//コンソールウィンドウの作成
	ConsoleWindow::Create();

	//スマートポインタ宣言
	std::unique_ptr<GameFrame>frame;

	//スマートポインタ
	frame = std::make_unique<VanishingSongFrame>();
	
	//ウィンドウクラスのオブジェクト作成
	WindowBase* window = new AetherWindow();

	//ウィンドウスタイルの設定
	WindowBase::WindowStyleDesc desc;
	desc._windowStyle = WS_OVERLAPPED | WS_SYSMENU;

	//ウィンドウスタイルをウィンドウオブジェクトに設定
	window->SetWindowStyles(desc);

	//ウィンドウを作成しているウィンドウのオブジェクトのメンバ関数
	window->Create(kWindowName, Vector2(kWindowWidth, kWindowHeight));

	bool result = frame->Initialize(&window,kWindowNum,5000.0f,1.0f);

	//エラーチェック
	if (!result){
		return kError;
	}

	//一番最初のシーンとロード画面の設定
	GameSceneManager::SetCurrentScene(new SceneTitle());
	GameSceneManager::SetLoadScreen(new Load());
	frame->BackgroundColor(Color(0, 0, 0, 0));
	//メインループの開始
	frame->GameRun();

	delete window;
	window = nullptr;

	//フレームオブジェクトの解放
	frame->Finalize();
	frame.release();
	//コンソールウィンドウの破棄
	ConsoleWindow::Close();
	return kEnd;
}
