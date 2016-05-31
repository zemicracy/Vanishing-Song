#pragma once
/*
	暗号化を解くよう
*/
#include <string>
#include <unordered_map>

class Cipher
{
private:
	typedef std::unordered_map<std::string, std::vector<std::string>> CipherType;
public:
	Cipher(std::string);
	~Cipher();

	void mConsoleFind();
	std::string mGetData(std::string key,const int arrayID);
	int mGetDataSize(std::string key);
	std::vector<std::string>& mGetSpriteArray(std::string key);
private:
	void mLoadFile(std::string);
	std::string mDecode(std::string,int& KeyCount);
	void mGetData(CipherType& hash,std::string tag,std::string data,const char spritChar);
	std::string mCheck(std::string key);
private:
	/*
	   キー用配列
	   内部に保持するデータは16進数
	*/
	static const int mKey[]; 

	static const int mKeySize;
	CipherType m_elemetHash;
};

