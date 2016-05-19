#include "ResourceManager.h"
#include "Debug.h"
#include"ActionSound.h"
#include "PixelShader.h"
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
	"Sound/BGM.wav",
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
	InitializeShader();
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
	return;
}


std::shared_ptr<ActionSound> ResourceManager::GetActionSound(eCommandType type){
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
		}else
		index->SetValume(-2000);
	}

	return true;
}


/*
	アクションコマンドに対応する音の初期化
*/
bool ResourceManager::InitializeActionSound(){
	return true;
}


/*
	テクスチャの初期化
*/
bool ResourceManager::InitializeTexture(){
	RegisterTexture("skybox", "Texture\\Game\\GameBack.jpg");
	RegisterTexture("0", "Texture\\Number\\0.png");
	RegisterTexture("1", "Texture\\Number\\1.png");
	RegisterTexture("2", "Texture\\Number\\2.png");
	RegisterTexture("3", "Texture\\Number\\3.png");
	RegisterTexture("4", "Texture\\Number\\4.png");
	RegisterTexture("5", "Texture\\Number\\5.png");
	RegisterTexture("6", "Texture\\Number\\6.png");
	RegisterTexture("7", "Texture\\Number\\7.png");
	RegisterTexture("8", "Texture\\Number\\8.png");
	RegisterTexture("9", "Texture\\Number\\9.png");
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
	for (auto& index : m_pBaseBgmArray)
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
bool ResourceManager::RegisterActionSound(eCommandType type, std::string path){
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
void ResourceManager::mPlayerInitialize(eMusical type, std::string directy, std::string tex){
	std::shared_ptr<Gear> gear;
	if (m_pPlayerHashes.find(type) != m_pPlayerHashes.end() || type == eMusical::eNull)return;
	m_pPlayerHashes[type] = std::make_shared<GearFrame>();

	// 体のパーツ
	m_pPlayerHashes[type]->m_pBody = m_charaEntity.mSetUpGear(directy + "\\body.fbx", Gear::eType::eBody, directy + tex);

	// 腰のパーツ
	m_pPlayerHashes[type]->m_pWaist = m_charaEntity.mSetUpGear(directy + "\\waist.fbx", Gear::eType::eWaist, directy + tex);

	// 腕のパーツ
	m_pPlayerHashes[type]->m_pLeftUpperArm = m_charaEntity.mSetUpGear(directy + "\\arm1.fbx", Gear::eType::eLeftUpperArm, directy + tex);
	m_pPlayerHashes[type]->m_pRightUpperArm = m_charaEntity.mSetUpGear(directy + "\\arm1.fbx", Gear::eType::eRightUpperArm, directy + tex);
	m_pPlayerHashes[type]->m_pLeftLowerArm = m_charaEntity.mSetUpGear(directy + "\\arm2.fbx", Gear::eType::eLeftLowerArm, directy + tex);
	m_pPlayerHashes[type]->m_pRightLowerArm = m_charaEntity.mSetUpGear(directy + "\\arm2.fbx", Gear::eType::eRightLowerArm, directy + tex);

	// 手のパーツ
	m_pPlayerHashes[type]->m_pLeftHand = m_charaEntity.mSetUpGear(directy + "\\hand.fbx", Gear::eType::eLeftHand, directy + tex);
	m_pPlayerHashes[type]->m_pRightHand = m_charaEntity.mSetUpGear(directy + "\\hand.fbx", Gear::eType::eRightHand, directy + tex);

	// 足のパーツ
	m_pPlayerHashes[type]->m_pLeftUpperLeg = m_charaEntity.mSetUpGear(directy + "\\leg1.fbx", Gear::eType::eLeftUpperLeg, directy + tex);
	m_pPlayerHashes[type]->m_pRightUpperLeg = m_charaEntity.mSetUpGear(directy + "\\leg1.fbx", Gear::eType::eRightUpperLeg, directy + tex);
	m_pPlayerHashes[type]->m_pLeftLowerLeg = m_charaEntity.mSetUpGear(directy + "\\leg2.fbx", Gear::eType::eLeftLowerLeg, directy + tex);
	m_pPlayerHashes[type]->m_pRightLowerLeg = m_charaEntity.mSetUpGear(directy + "\\leg2.fbx", Gear::eType::eRightLowerLeg, directy + tex);

	m_pPlayerHashes[type]->m_pLeftFoot = m_charaEntity.mSetUpGear(directy + "\\foot.fbx", Gear::eType::eLeftFoot, directy + tex);
	m_pPlayerHashes[type]->m_pRightFoot = m_charaEntity.mSetUpGear(directy + "\\foot.fbx", Gear::eType::eRightFoot, directy + tex);

	// それぞれのパーツとの親子関係構築
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pBody, m_pPlayerHashes[type]->m_pWaist);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pBody, m_pPlayerHashes[type]->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pBody, m_pPlayerHashes[type]->m_pLeftUpperArm);

	// 右
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pRightUpperArm, m_pPlayerHashes[type]->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pRightLowerArm, m_pPlayerHashes[type]->m_pRightHand);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pWaist, m_pPlayerHashes[type]->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pRightUpperLeg, m_pPlayerHashes[type]->m_pRightLowerLeg);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pRightLowerLeg, m_pPlayerHashes[type]->m_pRightFoot);

	// 左
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pLeftUpperArm, m_pPlayerHashes[type]->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pLeftLowerArm, m_pPlayerHashes[type]->m_pLeftHand);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pWaist, m_pPlayerHashes[type]->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pLeftUpperLeg, m_pPlayerHashes[type]->m_pLeftLowerLeg);
	m_charaEntity.mCreateRelationship(m_pPlayerHashes[type]->m_pLeftLowerLeg, m_pPlayerHashes[type]->m_pLeftFoot);

	return;
}

// プレイヤー取得用
std::shared_ptr<GearFrame> ResourceManager::mGetPlayerHash(eMusical type){
	return m_pPlayerHashes[type];
}

// 解放処理
void ResourceManager::FinalizePlayer(){
	for (auto& index : m_pPlayerHashes){
		index.second->Release();
	}
}

// 雑魚敵用
void ResourceManager::mEnemyInitialize(eMusical type, std::string directy){
	std::shared_ptr<Gear> gear;
	if (m_pEnemyHashes.find(type) != m_pEnemyHashes.end() || type == eMusical::eNull)return;
	
	m_pEnemyHashes[type] = std::make_shared<GearFrame>();

		// 体のパーツ
	m_pEnemyHashes[type]->m_pBody = m_charaEntity.mSetUpGear(directy + "\\body.fbx", Gear::eType::eBody, directy + "\\tex");

		// 腰のパーツ
	m_pEnemyHashes[type]->m_pWaist = m_charaEntity.mSetUpGear(directy + "\\waist.fbx", Gear::eType::eWaist, directy + "\\tex");


		// それぞれのパーツとの親子関係構築
	m_charaEntity.mCreateRelationship(m_pEnemyHashes[type]->m_pBody, m_pEnemyHashes[type]->m_pWaist);
}

//
std::shared_ptr<GearFrame> ResourceManager::mGetEnemyHash(eMusical type){
	return m_pEnemyHashes[type];
}

// 解放処理
void ResourceManager::FinalizeEnemy(){
	for (auto& index : m_pEnemyHashes){
		if (!index.second)continue;
		index.second->Release();
	}
}