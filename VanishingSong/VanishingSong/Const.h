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
	const float kWindowWidth = 1280.0f;
	const float kWindowHeight = 720.0f;

	// 初期化用
	const aetherClass::Vector3 kVector3Zero = aetherClass::Vector3(NULL, NULL, NULL);

	// Editorとの連携用
	const float kResolutionFromEditor_x = 1.6f;
	const float kResolutionFromEditor_y = 1.2f;

	// デバッグ関係
	const bool kCharaDebug = true;

	// 空間分割の番号
	const int kPartition1 = 0;
	const int kPartition2 = 1;
	const int kPartition3 = 2;
	const int kPartition4 = 3;

	const int kPartitionSize = 4;

	// ゲームにかかわる定数
	const signed char kFirstDay = 1;
	const signed char kLastDay = 1;
}

enum class eMusical{
	eNull,
	eRed,
	eBlue,
	eYellow,
	eGreen
};


#endif