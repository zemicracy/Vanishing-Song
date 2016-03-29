/*
	�f�o�b�O���Ƃ����o���Ƃ��͂��̃N���X�̂��g���܂��傤
	�ȉ��A�g����
	Debug::mErrorPrint("�G���[���e",__FILE__,__LINE__); // �R���\�[�����[�h
	Debug::mErrorPrint("�G���[���e",__FILE__,__LINE__,Debug::eState::eWindow); // �E�B���h�E���[�h

	Debug::mPrint("�f�o�b�O�����e"); // �R���\�[�����[�h
	Debug::mPrint("�f�o�b�O�����e",Debug::eState::eWindow); // �E�B���h�E���[�h

*/
#ifndef _DEBUG_H
#define _DEBUG_H
#include <string>
class Debug
{
public:
	enum class eState{
		eConsole,
		eWindow,
		eNull
	};

	static void mErrorPrint(std::string errorMessgae, std::string FaileName, const int line, const eState state = eState::eConsole);
	static void mPrint(std::string messgae, const eState state = eState::eConsole);

private:
	Debug() = default;
	~Debug() = default;


};

#endif