#include "ResourceManager.h"
#include "Debug.h"
#include"ActionSound.h"
using namespace aetherClass;
ResourceManager::ResourceManager()
{
	m_pActionSoundHash.clear();
	m_pTextureHash.clear();
}


ResourceManager::~ResourceManager()
{
}

/*
	BGMのパス情報配列
	BGMを変えたい場合はここをいじってね
*/
std::string ResourceManager::m_BgmPath[kMaxBGM] = {
	"null",
	"null",
	"null",
	"null",
	"null",
	"null",
	"null",
	"null",
};
/*
	リソース系の初期化処理
*/
bool ResourceManager::Initialize(){
	Finalize();

	InitializeBGM();
	InitializeActionSound();
	InitializeTexture();
	return true;
}

/*
	リソース系の解放処理
*/
void ResourceManager::Finalize(){

	FinalizeTexture();
	FinalizeSound();
	FinalizeBGM();
	
	return;
}


std::shared_ptr<ActionSound> ResourceManager::GetActionSound(eActionType type){
	return m_pActionSoundHash[type];
}


/*
	基本BGMを流す奴

*/
void ResourceManager::PlayBaseBGM(const int id){
	m_pBaseBgmArray[id]->PlayToLoop();
	return;
}

/*
	テクスチャの取得用
*/
std::shared_ptr<aetherClass::Texture> ResourceManager::GetTexture(std::string name){
	return m_pTextureHash[name];
}

/*
基本BGMの初期化

*/
bool ResourceManager::InitializeBGM(){

	int now = 0;
	bool result = false;
	for (auto& index : m_pBaseBgmArray)
	{
		index = std::make_shared<GameSound>();
		result = index->Load(m_BgmPath[now].c_str());
		now += 1;
		if (!result)
		{
			Debug::mErrorPrint("BGMの読み込みに失敗しました", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		}
	}

	return true;
}


/*
	アクションコマンドに対応する音の初期化
*/
bool ResourceManager::InitializeActionSound(){
	RegisterActionSound(eActionType::eShortDistanceAttack, "Sound/damage03.wav");
	return true;
}


/*
	テクスチャの初期化
*/
bool ResourceManager::InitializeTexture(){
	return true;
}

/*
BGMの配列の要素を削除
*/
void ResourceManager::FinalizeBGM(){
	for (auto index : m_pBaseBgmArray)
	{
		if (!index)continue;
		index.reset();
		index = nullptr;
	}
	m_pBaseBgmArray.fill(nullptr);
	return;
}


/*
	アクションコマンドに対応する連想配列の解放処理
*/
void ResourceManager::FinalizeSound(){
	for (auto index : m_pActionSoundHash)
	{
		if (!index.second)continue;
		index.second.reset();
		index.second = nullptr;
	}

	m_pActionSoundHash.clear();
	return;
}


/*
	テクスチャの配列を削除
*/
void ResourceManager::FinalizeTexture(){
	for (auto index : m_pTextureHash)
	{
		if (!index.second)continue;
		index.second.reset();
		index.second = nullptr;
	}
	m_pTextureHash.clear();
	return;
}

/*
	アクションコマンドに対応したサウンドの登録	
*/
bool ResourceManager::RegisterActionSound(eActionType type, std::string path){
	auto findMap = m_pActionSoundHash.find(type);

	// すでにその名前で登録しているのであれば何もしない
	if (findMap != m_pActionSoundHash.end())
	{
		Debug::mErrorPrint("既に登録済みのキーのため登録ができませんでした", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		return false;
	}

	std::shared_ptr<ActionSound> action;
	action = std::make_shared<ActionSound>();
	action->mLoad(path);
	m_pActionSoundHash.insert(std::make_pair(type, action));
	return true;
}

/*
	テクスチャの登録用
*/
bool ResourceManager::RegisterTexture(std::string registerName, std::string path){
	
	auto findMap = m_pTextureHash.find(registerName);

	// すでにその名前で登録しているのであれば何もしない
	if (findMap != m_pTextureHash.end())
	{
		Debug::mErrorPrint("既に登録済みのキーのため登録ができませんでした", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		return false;
	}

	// 登録処理
	std::shared_ptr<Texture> texture;
	texture->Load(path);
	m_pTextureHash.insert(std::make_pair(registerName, texture));

	return true;
}
