#include "ResourceManager.h"
#include "Debug.h"
#include"ActionSound.h"
#include "PixelShader.h"
namespace{
	const int kMaxBGM = 4;
}
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


/*
	リソース系の初期化処理
*/
bool ResourceManager::Initialize(){
	Finalize();

	InitializeBGM();
	InitializeActionSound();
	InitializeTexture();
	InitializeShader();
	mInitializeLaod();

	return true;
}

/*
	リソース系の解放処理
*/
void ResourceManager::Finalize(){
	FinalizePlayer();
	FinalizeEnemy();
	FinalizeTexture();
	FinalizeSound();
	FinalizeBGM();
	InitializeShader();
	mFinalizeLoad();
	return;
}

ResourceManager& ResourceManager::mGetInstance(){
	static ResourceManager instance;
	return instance;
}

std::shared_ptr<ActionSound> ResourceManager::GetActionSound(eMusical type){
	return m_pActionSoundHash[type];
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

	bool result = false;
	return true;
}


/*
	アクションコマンドに対応する音の初期化
*/
bool ResourceManager::InitializeActionSound(){
	RegisterActionSound(eMusical::eBlue, "Sound/blue.wav");
	RegisterActionSound(eMusical::eGreen, "Sound/green.wav");
	RegisterActionSound(eMusical::eRed, "Sound/red.wav");
	RegisterActionSound(eMusical::eYellow, "Sound/yellow.wav");
	RegisterActionSound(eMusical::eMiss, "Sound/miss.wav");

	return true;
}


/*
	テクスチャの初期化
*/
bool ResourceManager::InitializeTexture(){
	RegisterTexture("skybox", "Texture\\Game\\GameBack.jpg");

	RegisterTexture("comment", "Texture\\Message\\comment_flame.png");
	RegisterTexture("comment2", "Texture\\Message\\comment_flame2.png");

	std::string comPath = "Texture\\ActionCommand\\";
	RegisterTexture("ActionBlue", comPath + "Blue.png");
	RegisterTexture("ActionGreen", comPath + "Green.png");
	RegisterTexture("ActionRed", comPath + "Red.png");
	RegisterTexture("ActionYellow", comPath + "Yellow.png");
	RegisterTexture("ActionMiss",comPath + "miss.png");
	RegisterTexture("ActionNull",comPath + "null.png");
	RegisterTexture("ActionAdlib", comPath + "adlib.png");
	return true;
}

/*
シェーダーの初期化
*/
bool ResourceManager::InitializeShader(){
	ShaderDesc desc;
	desc._vertex._entryName = "vs_main";
	desc._pixel._entryName = "ps_main";

	desc._vertex._srcFile = L"Shader\\VertexShaderBase.hlsl";
	desc._pixel._srcFile = L"Shader\\BasicColor.hlsl";
	RegisterShader<PixelShader>("color", desc);

	desc._pixel._srcFile = L"Shader\\Texture.hlsl";
	RegisterShader<PixelShader>("texture", desc);

	desc._pixel._srcFile = L"Shader\\Transparent.hlsl";
	RegisterShader<PixelShader>("transparent", desc);
	return true;
}


/*
BGMの配列の要素を削除
*/
void ResourceManager::FinalizeBGM(){
	m_BgmPath.clear();
	return;
}


/*
	アクションコマンドに対応する連想配列の解放処理
*/
void ResourceManager::FinalizeSound(){
	for (auto& index : m_pActionSoundHash)
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
	for (auto& index : m_pTextureHash)
	{
		if (!index.second)continue;
		index.second.reset();
		index.second = nullptr;
	}
	m_pTextureHash.clear();
	return;
}

void ResourceManager::FinalizeSahder(){
	for (auto& index : m_pShaderHash)
	{
		if (!index.second)continue;
		index.second->Finalize();
		index.second.reset();
		index.second = nullptr;
	}
	m_pShaderHash.clear();
	return;
}

/*
	アクションコマンドに対応したサウンドの登録	
*/
bool ResourceManager::RegisterActionSound(eMusical type, std::string path){
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
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->Load(path);
	m_pTextureHash.insert(std::make_pair(registerName, texture));

	return true;
}



template<class Type>
std::shared_ptr<Type> ResourceManager::RegisterShader(std::string registerName, ShaderDesc desc){
	auto findMap = m_pTextureHash.find(registerName);

	// すでにその名前で登録しているのであれば何もしない
	if (findMap != m_pTextureHash.end())
	{
		Debug::mErrorPrint("既に登録済みのキーのため登録ができませんでした", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		return nullptr;
	}

	// 登録処理
	std::shared_ptr<Type> shader = std::make_shared<Type>();
	bool result = shader->Initialize(desc, ShaderType::eVertex | ShaderType::ePixel);
	if (!result){
		Debug::mErrorPrint("初期化に失敗", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		return false;
	}
	m_pShaderHash.insert(std::make_pair(registerName, shader));

	return shader;
}

std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>>& ResourceManager::mGetShaderHash(){
	return m_pShaderHash;
}

// プレイヤー系初期化用
void ResourceManager::mPlayerInitialize(eMusical type, std::string path, std::string tex){
	if (m_pPlayerHashes.find(type) != m_pPlayerHashes.end() || type == eMusical::eNull)return;
	m_pPlayerHashes[type] = std::make_shared<FbxModel>();

	// 体のパーツ
	m_pPlayerHashes[type]->LoadFBX(path+"\\test.fbx", eAxisSystem::eAxisOpenGL);
	m_pPlayerHashes[type]->SetTextureDirectoryName(tex);
	return;
}

// プレイヤー取得用
std::shared_ptr<FbxModel> ResourceManager::mGetPlayerHash(eMusical type){
	return m_pPlayerHashes[type];
}

// 解放処理
void ResourceManager::FinalizePlayer(){
	for (auto& index : m_pPlayerHashes){
		index.second->Finalize();
	}
	m_pPlayerHashes.clear();
}

// 雑魚敵用
void ResourceManager::mEnemyInitialize(eMusical type, std::string directry, std::string tex){
	if (type == eMusical::eNull)return;
	
	m_pEnemyHashes[type] = std::make_shared<FbxModel>();
	m_pEnemyHashes[type]->LoadFBX(directry + "\\body.fbx", eAxisSystem::eAxisOpenGL);
	m_pEnemyHashes[type]->SetTextureDirectoryName(directry+tex);
}


//敵の取得
std::shared_ptr<FbxModel> ResourceManager::mGetEnemyHash(eMusical type){
	if (m_pEnemyHashes.find(type) == m_pEnemyHashes.end())return nullptr;
	return m_pEnemyHashes.at(type);
}

// 解放処理
void ResourceManager::FinalizeEnemy(){
	for (auto& index : m_pEnemyHashes){
		if (index.second){
			index.second->Finalize();
		}
	}
}

std::unordered_map<eMusical,std::string>& ResourceManager::mGetBGMPath(){
	return m_BgmPath;
}
std::string& ResourceManager::mSetBGMPath(eMusical type){
	return m_BgmPath[type];
}

void ResourceManager::mInitializeLaod(){
	for (int i = 0; i < 3; ++i){
		RegisterTexture("NowLoading" + std::to_string(i + 1), "Texture\\Load\\NowLoading\\" + std::to_string(i + 1) + ".png");
	}
}

//
void ResourceManager::mFinalizeLoad(){

}
