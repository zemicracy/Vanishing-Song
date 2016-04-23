#ifndef _CONST_H
#define _CONST_H
#include <Vector3.h>
#include "Debug.h"
#include <string>
/*
	定数やマクロ関数を定義しておくファイル

*/
namespace{
	// ウィンドウ関連
	const std::wstring kWindowName = L"Game";
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// 初期化用
	const aetherClass::Vector3 kVector3Zero = aetherClass::Vector3(NULL, NULL, NULL);

	// Editorとの連携用
	const float kResolutionFromEditor_x = 1.6;
	const float kResolutionFromEditor_y = 1.2;

	// デバッグ関係
	const bool kCharaDebug = true;

	// 空間分割の番号
	const int kPartition1 = 0;
	const int kPartition2 = 1;
	const int kPartition3 = 2;
	const int kPartition4 = 3;


	// ゲームにかかわる定数
	const signed char kFirstDay = 1;
	const signed char kLastDay = 1;

}
#endif