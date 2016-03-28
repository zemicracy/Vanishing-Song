#ifndef _AETHER_H
#define _AETHER_H
#pragma comment (lib,"Aether.lib")
#endif
// 標準
#include <memory>
#include <windows.h>

// 独自フレームワーク
#include <GameEntry.h>
#include <GameFrame.h>
#include <ConsoleWindow.h>
#include <WindowBase.h>
#include <GameSceneManager.h>
using namespace aetherClass;

#include "WorldReader.h"
namespace{
	const int kExit = 0;
	const int kError = -1;
}

INT WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE, LPSTR, INT){
	ConsoleWindow::Create();

	ConsoleWindow::Close();
	return kExit;
}