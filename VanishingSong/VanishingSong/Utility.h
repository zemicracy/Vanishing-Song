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
	��ԗp�֐�
	�������F�������
	�������F�ő�̏��
	��O�����F�S�t���[����
	��l�����F���̃t���[����
*/
template<class type>
static type gInterpolation(type first, type last, const int allFrameCount, const int nowCount){
	
	// �ő�t���[�������z�������Ȃ�Ō�̏�Ԃ�Ԃ�
	if (allFrameCount <= nowCount) return last;
	

	type output;
	float allFrame = static_cast<float>(allFrameCount);
	float nowFrame = static_cast<float>(nowCount);
	float coefficient = (float)(nowFrame / allFrame);
	output = (type)(first + (last - first)*coefficient);
	return output;
}


/*
�}�E�X�̌Œ��A�����悤
*/
static bool gLockMouseCursor(HWND hWnd, const bool lock){
	RECT screen;
	BOOL result = GetWindowRect(hWnd, &screen);
	if (!result){
		Debug::mErrorPrint("�E�B���h�E�T�C�Y�̎擾�Ɏ��s", __FILE__, __LINE__);
		return false;
	}
	POINT screenCenter = { (screen.left + screen.right) / 2, (screen.top + screen.bottom) / 2 };

	// �}�E�X���Œ�
	if (lock)
	{
		SetCursorPos(screenCenter.x, screenCenter.y);
	}

	// �����Ń}�E�X�̐؂�ւ����s��
	//ShowCursor(visible);
	return true;
}
#endif