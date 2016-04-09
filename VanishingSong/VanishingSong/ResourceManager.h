#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H
#include "CharaStatus.h"
#include <unordered_map>
#include <memory>
#include <array>
#include <GameSound.h>
#include <Texture.h>
namespace{
	const int kMaxBGM = 8;
}
class ActionSound;
class ResourceManager
{
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
		アクションコマンドに対応した音の登録用
	*/
	bool RegisterActionSound(eCommandType, std::string path);

	/*
		テクスチャの登録用
	*/
	bool RegisterTexture(std::string registerName,std::string path);
private:
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>> m_pTextureHash;
	std::unordered_map<eCommandType, std::shared_ptr<ActionSound>> m_pActionSoundHash;
	std::array<std::shared_ptr<aetherClass::GameSound>,kMaxBGM> m_pBaseBgmArray;
	static std::string m_BgmPath[kMaxBGM];
};

#endif