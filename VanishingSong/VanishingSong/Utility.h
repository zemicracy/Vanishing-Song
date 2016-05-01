#ifndef _UTILITY_H
#define _UTILITY_H

#include <vector>
#include "GearFrame.h"
#include <Matrix4x4.h>
#include "Debug.h"
template<typename Type>
static void ReleaseVector(std::vector<Type>& vector)
{
	if (!vector.empty())
	{
		
		for (auto object : vector)
		{
			if (object)
			{
				delete object;
				object = nullptr;
			}
		}
	}
	vector.clear();
}

/*
	補間用関数
	第一引数：初期状態
	第二引数：最大の状態
	第三引数：全フレーム数
	第四引数：今のフレーム数
*/
template<class type>
static type gInterpolation(type first, type last, const int allFrameCount, const int nowCount){
	
	// 最大フレーム数を越えそうなら最後の状態を返す
	if (allFrameCount <= nowCount) return last;
	

	type output;
	float allFrame = static_cast<float>(allFrameCount);
	float nowFrame = static_cast<float>(nowCount);
	float coefficient = (float)(nowFrame / allFrame);
	output = (type)(first + (last - first)*coefficient);
	return output;
}


/*
マウスの固定や、消すよう
*/
static bool gLockMouseCursor(HWND hWnd, const bool lock){
	RECT screen;
	BOOL result = GetWindowRect(hWnd, &screen);
	if (!result){
		Debug::mErrorPrint("ウィンドウサイズの取得に失敗", __FILE__, __LINE__);
		return false;
	}
	POINT screenCenter = { (screen.left + screen.right) / 2, (screen.top + screen.bottom) / 2 };

	// マウスを固定
	if (lock)
	{
		SetCursorPos(screenCenter.x, screenCenter.y);
	}

	// ここでマウスの切り替えを行う
	//ShowCursor(visible);
	return true;
}
#endif