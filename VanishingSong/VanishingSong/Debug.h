/*
	デバッグ文とかを出すときはこのクラスのを使いましょう
	以下、使い方
	Debug::mErrorPrint("エラー内容",__FILE__,__LINE__); // コンソールモード
	Debug::mErrorPrint("エラー内容",__FILE__,__LINE__,Debug::eState::eWindow); // ウィンドウモード

	Debug::mPrint("デバッグ文内容"); // コンソールモード
	Debug::mPrint("デバッグ文内容",Debug::eState::eWindow); // ウィンドウモード

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