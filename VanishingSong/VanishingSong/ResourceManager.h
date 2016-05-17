#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H
#include <unordered_map>
#include <memory>
#include <array>
#include <GameSound.h>
#include <Texture.h>
#include <ShaderBase.h>
#include <Singleton.h>
#include "CharaEntity.h"
#include "GearFrame.h"
#include "CharaStatus.h"
#include "Const.h"
namespace{
	const int kMaxBGM = 8;
}
class ActionSound;

class ResourceManager
{

public:
	enum class eEnemyType{
		eNull, eAir, eGround
	};
	typedef std::unordered_map<eMusical, std::shared_ptr<GearFrame>> PlayerHash;
	typedef std::unordered_map<ResourceManager::eEnemyType, std::vector<std::shared_ptr<GearFrame>>> EnemyHash;
public:
	ResourceManager();
	~ResourceManager();

	// 全体の初期化
	bool Initialize();

	// 全体の解放
	void Finalize();
	
	// 基本BGMを流すよう
	void PlayBaseBGM(const int id);

	// テクスチャ取得用
	std::shared_ptr<aetherClass::Texture> GetTexture(std::string);

	std::shared_ptr<ActionSound> GetActionSound(eCommandType);

	// 基本的に使うシェーダーの取得用
	std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>>& mGetShaderHash();

	void mPlayerInitialize(eMusical,std::string directy);
	std::shared_ptr<GearFrame> mGetPlayerHash(eMusical);

	void mEnemyInitialize(eEnemyType, std::string directy);
	std::vector<std::shared_ptr<GearFrame>>& mGetEnemyHash(eEnemyType);
private:

	/*
		アクションコマンドに対応したサウンドの初期化
	*/
	bool InitializeActionSound();

	/*
		BGMの初期化
	*/
	bool InitializeBGM();

	/*
		テクスチャの初期化
	*/
	bool InitializeTexture();

	/*
	テクスチャの初期化
	*/
	bool InitializeShader();


	/*
		アクションコマンドに対応した音の解放処理
	*/
	void FinalizeSound();

	/*
		BGMの解放処理
	*/
	void FinalizeBGM();

	/*
		テクスチャの解放処理
	*/
	void FinalizeTexture();

	/*
	シェーダーの解放処理
	*/
	void FinalizeSahder();

	void FinalizePlayer();
	/*
		アクションコマンドに対応した音の登録用
	*/
	bool RegisterActionSound(eCommandType, std::string path);

	/*
		テクスチャの登録用
	*/
	bool RegisterTexture(std::string registerName,std::string path);

	/*
		シェーダーの登録用
	*/
	template<class Type>
	std::shared_ptr<Type> RegisterShader(std::string registerName, aetherClass::ShaderDesc);
private:
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>> m_pTextureHash;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>> m_pShaderHash;

	std::unordered_map<eCommandType, std::shared_ptr<ActionSound>> m_pActionSoundHash;
	std::array<std::shared_ptr<aetherClass::GameSound>,kMaxBGM> m_pBaseBgmArray;
	static std::string m_BgmPath[kMaxBGM];

	CharaEntity m_charaEntity;

	PlayerHash m_pPlayerHashes;
	EnemyHash m_pEnemyHashes;
};

#endif