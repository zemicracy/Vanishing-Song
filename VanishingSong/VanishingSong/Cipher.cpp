#include "Cipher.h"
#include <fstream>
#include <sstream>
#include "Debug.h"
const int Cipher::mKey[]{
		0x3727ba7,
		0x543a746,
		0x4278b82,
		0xca89431,
		0x42de281,
		0x3a8a43d
};

const int Cipher::mKeySize = 5;
Cipher::Cipher(std::string path)
{
	mLoadFile(path);
}


Cipher::~Cipher()
{
	mUnLoad();
}

void Cipher::mLoadFile(std::string path){
	std::ifstream reader;
	reader.open(path, std::ios::in);
	//reader.unsetf(std::ios::skipws);

	int KeyCount = NULL;
	std::string tag = "none";
	while (!reader.eof())
	{
		std::string readLine; // �ǂݎ��p
		
		std::getline(reader, readLine);
		
		
		std::string decode = mDecode(readLine, KeyCount);
		if (decode.size()>0&&(decode.front() == '['&&decode.back() == ']')){
			//�@�^�O�̐ݒ�
			tag = decode;
			m_elemetHash[tag].clear();
			continue;
		}
		else
		if (tag != "none"){
			// �^�O���s���Ȓl����Ȃ�������ǂݍ���
			mGetData(m_elemetHash, tag, decode,(','));
		}
	}
}

void Cipher::mUnLoad(){
	for (auto& index:m_elemetHash){
		index.second.clear();
	}
	m_elemetHash.clear();
}
//
void Cipher::mGetData(CipherType& hash, std::string tag, std::string data, const char spritChar){
	//
	if (data.empty())return;
	std::string split;
	std::vector<std::string> spliteArray;
	std::stringstream stream(data);

	// �w�蕶���ŋ�؂�
	while (std::getline(stream, split, spritChar))
	{
		spliteArray.push_back(split);
	}

	// �J���}��؂�œǂݍ���
	for (auto index : spliteArray){
		hash[tag].push_back(index);
	}
}

// �Í��������p
std::string Cipher::mDecode(std::string readLine, int& KeyCount){
	std::string decipher; // �Í������Ƃ���������擾�p
	for (auto readIndex : readLine){

		char decipherIndex = readIndex ^ mKey[KeyCount];

		decipher.push_back(decipherIndex);
		
		KeyCount += 1;
		if (KeyCount > mKeySize){
			KeyCount = NULL;
		}
	}
	return decipher;
}

/*
	�Ή�����L�[���Ȃ��������̓L�[�͂��邪�z��ɉ����Ȃ��Ƃ���null��Ԃ�
*/
std::string Cipher::mGetData(std::string key, const int arrayID){
	
	std::string ReKey = mCheck(key);

	if (m_elemetHash.find(ReKey) == m_elemetHash.end()||
		m_elemetHash[ReKey].size()<arrayID)return "null";
	return m_elemetHash[key][arrayID];
}

//
int Cipher::mGetDataSize(std::string key){
	std::string ReKey = mCheck(key);
	if (m_elemetHash.find(ReKey) == m_elemetHash.end())return NULL;
	return m_elemetHash[ReKey].size();
}
	

std::vector<std::string>& Cipher::mGetSpriteArray(std::string key){
	return m_elemetHash[key];
}

//
std::string Cipher::mCheck(std::string key){
	std::string ReKey = key;
	
	// []���Ȃ�������Ƃ肠�����A���������̂Ŕ��f����
	if (ReKey.front() != '[' || ReKey.back() != ']'){
		ReKey = "[" + key + "]";
	}
	return ReKey;
}

void Cipher::mConsoleFind(){
	for (auto& hash : m_elemetHash){
		Debug::mPrint("Key :" +hash.first);
		for (auto& data : hash.second){
			Debug::mPrint("data :" + data);
		}
	}
	return;
}

void Cipher::mLock(std::string filePath){


	std::ifstream reader;
	reader.open(filePath, std::ios::in);

	reader.unsetf(std::ios::skipws);

	std::vector<std::string> input;

	int KeyCounter = 0;
	while (!reader.eof())
	{
		std::string line;
		std::getline(reader, line);

		std::string data;

		input.push_back(line);
	}
	reader.close();

	std::ofstream wrriter;
	std::vector<std::string> pathSplite;
	std::stringstream ss(filePath);
	std::string buffer;
	while (std::getline(ss, buffer, '.')) {
		pathSplite.push_back(buffer);
	}

	wrriter.open(filePath);
	std::string aether = pathSplite.back();
	int i = 0;
	KeyCounter = 0;
	for (auto index : input){
		for (auto moji : index){
			char ango = moji ^ mKey[KeyCounter];
			wrriter << ango;
			if (aether != "aether"){
				KeyCounter += 1;
				if (KeyCounter > mKeySize){
					KeyCounter = 0;
				}
			}
			else{
				KeyCounter = 0;
			}
		}

		i += 1;
		if (i > input.size() - 1){
			break;
		}

		wrriter << "\n";

	}


	wrriter.close();
}