#ifndef _CONST_H
#define _CONST_H
#include <Vector3.h>
#include "Debug.h"
#include <string>
/*
	�萔��}�N���֐����`���Ă����t�@�C��

*/
namespace{
	// �E�B���h�E�֘A
	const std::wstring kWindowName = L"Game";
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// �������p
	const aetherClass::Vector3 kVector3Zero = aetherClass::Vector3(NULL, NULL, NULL);

	// Editor�Ƃ̘A�g�p
	const float kResolutionFromEditor_x = 1.6;
	const float kResolutionFromEditor_y = 1.2;

	// �f�o�b�O�֌W
	const bool kCharaDebug = true;

	// �Q�[���ɂ������萔
	const signed char kFirstDay = 1;
	const signed char kLastDay = 1;

}
#endif