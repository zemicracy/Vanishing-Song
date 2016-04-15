#include "Debug.h"
#include <iostream>
#include <Windows.h>
#include <FbxUtility.h>

// ここのコメントdefineをコメントで隠せばデバッグ文でないよ
#ifndef DebugModeON
#define DebugModeON
#endif
using namespace aetherFunction;

void Debug::mErrorPrint(std::string errorMessgae, std::string FileName, const eState state){
#ifdef DebugModeON 1

	if (state == Debug::eState::eWindow){
		std::wstring convertMessage = StringToWString(errorMessgae) + L"/ファイル名 :" + StringToWString(StringSplite(FileName, '\\'));
		MessageBox(NULL, convertMessage.c_str(), L"DebugMode Error", MB_OK);
	}
	else if (state == Debug::eState::eConsole){

		std::cout << "DebugMdoe Error :" << errorMessgae << std::endl;
		std::cout << "ファイル名 :" << StringSplite(FileName, '\\') << std::endl;
	}

#endif
}

void Debug::mErrorPrint(std::string errorMessgae, std::string FileName, const int line, const eState state){

#ifdef DebugModeON 1
	
	if (state == Debug::eState::eWindow){
		std::wstring convertMessage = StringToWString(errorMessgae) + L"/ファイル名 :" + StringToWString(StringSplite(FileName,'\\')) + L"/行数 :" + std::to_wstring(line);
		MessageBox(NULL, convertMessage.c_str(), L"DebugMode Error", MB_OK);
	}
	else if (state == Debug::eState::eConsole){
		
		std::cout <<"DebugMode Error :"<< errorMessgae << std::endl;
		std::cout << "ファイル名 :" << StringSplite(FileName,'\\') << "/" << "行数 :" << line << std::endl;
	}

#endif
}



void Debug::mErrorPrint(std::string errorMessgae, std::string FileName, std::string FunctionName, const int line, const eState state){
#ifdef DebugModeON 1

	if (state == Debug::eState::eWindow){
		std::wstring convertMessage = StringToWString(errorMessgae) + L"/ファイル名 :" + StringToWString(StringSplite(FileName, '\\')) + L"/関数名 ："+StringToWString(FunctionName)+ L"/行数 :" + std::to_wstring(line);
		MessageBox(NULL, convertMessage.c_str(), L"DebugMode Error", MB_OK);
	}
	else if (state == Debug::eState::eConsole){

		std::cout << "DebugMdoe Error :" << errorMessgae << std::endl;
		std::cout << "ファイル名 :" << StringSplite(FileName, '\\') << "/"<<"関数名 ："<<FunctionName<<"/" << "行数 :" << line << std::endl;
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

