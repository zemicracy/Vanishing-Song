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
	const float kWindowWidth = 1280.0f;
	const float kWindowHeight = 720.0f;

	// �������p
	const aetherClass::Vector3 kVector3Zero = aetherClass::Vector3(NULL, NULL, NULL);

	// Editor�Ƃ̘A�g�p
	const float kResolutionFromEditor_x = 1.6f;
	const float kResolutionFromEditor_y = 1.2f;

	// �f�o�b�O�֌W
	const bool kCharaDebug = true;

	// ��ԕ����̔ԍ�
	const int kPartition1 = 0;
	const int kPartition2 = 1;
	const int kPartition3 = 2;
	const int kPartition4 = 3;

	const int kPartitionSize = 4;

	// �Q�[���ɂ������萔
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