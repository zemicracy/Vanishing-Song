#ifndef _AETHER_H
#define _AETHER_H
#pragma comment (lib,"Aether.lib")
#endif
#include <memory>
#include <GameFrame.h>
#include <AetherWindow.h>
#include <GameController.h>
#include <ConsoleWindow.h>
#include <GameSceneManager.h>
#include "SceneSurvival.h"

using namespace aetherClass;

namespace{
	const int kError = -1;
	const int kEnd = 0;
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;
	const int kWindowNum = 1;
}

// エントリーポイントを作成
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT){
	
	//コンソールウィンドウの作成
	ConsoleWindow::Create();

	//スマートポインタ宣言
	std::unique_ptr<GameFrame>frame;

	//スマートポインタ
	frame = std::make_unique<GameFrame>();
	
	//ウィンドウクラスのオブジェクト作成
	WindowBase* window = new AetherWindow();

	//ウィンドウスタイルの設定
	WindowBase::WindowStyleDesc desc;
	desc._windowStyle = WS_OVERLAPPED | WS_SYSMENU;

	//ウィンドウスタイルをウィンドウオブジェクトに設定
	window->SetWindowStyles(desc);

	//ウィンドウを作成しているウィンドウのオブジェクトのメンバ関数
	window->Create(L"Game", Vector2(kWindowWidth, kWindowHeight));

	bool result = frame->Initialize(&window,kWindowNum,5000.0f,1.0f);

	//エラーチェック
	if (!result){
		return kError;
	}

	//一番最初のシーンの設定
	GameScene* scene = new SceneSurvival;
	GameSceneManager::SetCurrentScene(scene);

	//メインループの開始
	frame->GameRun();

	delete window;
	window = nullptr;

	//フレームオブジェクトの解放
	frame->Finalize();

	//コンソールウィンドウの破棄
	ConsoleWindow::Close();
	return kEnd;
}
