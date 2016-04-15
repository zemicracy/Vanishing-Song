#include "Debug.h"
#include <iostream>
#include <Windows.h>
#include <FbxUtility.h>

// �����̃R�����gdefine���R�����g�ŉB���΃f�o�b�O���łȂ���
#ifndef DebugModeON
#define DebugModeON
#endif
using namespace aetherFunction;

void Debug::mErrorPrint(std::string errorMessgae, std::string FileName, const eState state){
#ifdef DebugModeON 1

	if (state == Debug::eState::eWindow){
		std::wstring convertMessage = StringToWString(errorMessgae) + L"/�t�@�C���� :" + StringToWString(StringSplite(FileName, '\\'));
		MessageBox(NULL, convertMessage.c_str(), L"DebugMode Error", MB_OK);
	}
	else if (state == Debug::eState::eConsole){

		std::cout << "DebugMdoe Error :" << errorMessgae << std::endl;
		std::cout << "�t�@�C���� :" << StringSplite(FileName, '\\') << std::endl;
	}

#endif
}

void Debug::mErrorPrint(std::string errorMessgae, std::string FileName, const int line, const eState state){

#ifdef DebugModeON 1
	
	if (state == Debug::eState::eWindow){
		std::wstring convertMessage = StringToWString(errorMessgae) + L"/�t�@�C���� :" + StringToWString(StringSplite(FileName,'\\')) + L"/�s�� :" + std::to_wstring(line);
		MessageBox(NULL, convertMessage.c_str(), L"DebugMode Error", MB_OK);
	}
	else if (state == Debug::eState::eConsole){
		
		std::cout <<"DebugMode Error :"<< errorMessgae << std::endl;
		std::cout << "�t�@�C���� :" << StringSplite(FileName,'\\') << "/" << "�s�� :" << line << std::endl;
	}

#endif
}



void Debug::mErrorPrint(std::string errorMessgae, std::string FileName, std::string FunctionName, const int line, const eState state){
#ifdef DebugModeON 1

	if (state == Debug::eState::eWindow){
		std::wstring convertMessage = StringToWString(errorMessgae) + L"/�t�@�C���� :" + StringToWString(StringSplite(FileName, '\\')) + L"/�֐��� �F"+StringToWString(FunctionName)+ L"/�s�� :" + std::to_wstring(line);
		MessageBox(NULL, convertMessage.c_str(), L"DebugMode Error", MB_OK);
	}
	else if (state == Debug::eState::eConsole){

		std::cout << "DebugMdoe Error :" << errorMessgae << std::endl;
		std::cout << "�t�@�C���� :" << StringSplite(FileName, '\\') << "/"<<"�֐��� �F"<<FunctionName<<"/" << "�s�� :" << line << std::endl;
	}

#endif
}


void Debug::mPrint(std::string messgae, const eState state){
#ifdef DebugModeON 1

	if (state == Debug::eState::eWindow){
		std::wstring convertMessage = StringToWString(messgae);
		MessageBox(NULL, convertMessage.c_str(), L"DebugMode", MB_OK);
	}
	else if (state == Debug::eState::eConsole){
		std::cout << "DebugMode Message:" << messgae << std::endl;
	}

#endif
}

