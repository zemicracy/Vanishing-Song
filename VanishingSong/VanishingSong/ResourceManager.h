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

class ActionSound;

class ResourceManager
{

public:
	typedef std::unordered_map<eMusical, std::shared_ptr<GearFrame>> CharaHash;
public:
	ResourceManager();
	~ResourceManager();

	// �S�̂̏�����
	bool Initialize();

	// �S�̂̉��
	void Finalize();
	
	// ��{BGM�𗬂��悤
	void mPlayBaseBGM(eMusical);

	void mStopBaseBGM(eMusical);
	// �e�N�X�`���擾�p
	std::shared_ptr<aetherClass::Texture> GetTexture(std::string);

	std::shared_ptr<ActionSound> GetActionSound(eMusical);
	std::shared_ptr<aetherClass::GameSound> mGetBGM(eMusical);

	// ��{�I�Ɏg���V�F�[�_�[�̎擾�p
	std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>>& mGetShaderHash();

	void mPlayerInitialize(eMusical, std::string directy, std::string tex);
	std::shared_ptr<GearFrame> mGetPlayerHash(eMusical);

	void mEnemyInitialize(eMusical, std::string directy, std::string tex);
	std::shared_ptr<GearFrame> mGetEnemyHash(eMusical);

	std::shared_ptr<aetherClass::GameSound> mGetLastBGM();
private:

	/*
		�A�N�V�����R�}���h�ɑΉ������T�E���h�̏�����
	*/
	bool InitializeActionSound();

	/*
		BGM�̏�����
	*/
	bool InitializeBGM();

	/*
		�e�N�X�`���̏�����
	*/
	bool InitializeTexture();

	/*
	�e�N�X�`���̏�����
	*/
	bool InitializeShader();


	/*
		�A�N�V�����R�}���h�ɑΉ��������̉������
	*/
	void FinalizeSound();

	/*
		BGM�̉������
	*/
	void FinalizeBGM();

	/*
		�e�N�X�`���̉������
	*/
	void FinalizeTexture();

	/*
	�V�F�[�_�[�̉������
	*/
	void FinalizeSahder();

	void FinalizePlayer();

	void FinalizeEnemy();
	/*
		�A�N�V�����R�}���h�ɑΉ��������̓o�^�p
	*/
	bool RegisterActionSound(eMusical, std::string path);

	/*
		�e�N�X�`���̓o�^�p
	*/
	bool RegisterTexture(std::string registerName,std::string path);

	/*
		�V�F�[�_�[�̓o�^�p
	*/
	template<class Type>
	std::shared_ptr<Type> RegisterShader(std::string registerName, aetherClass::ShaderDesc);
private:
	struct BGMType
	{
		std::string _path;
		eMusical _type;
	};
private:
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>> m_pTextureHash;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>> m_pShaderHash;

	std::unordered_map<eMusical, std::shared_ptr<ActionSound>> m_pActionSoundHash;
	std::unordered_map<eMusical, std::shared_ptr<aetherClass::GameSound>>m_pBaseBgmArray;
	static const BGMType m_BgmPath[4];

	std::shared_ptr<aetherClass::GameSound> m_pLastBGM;
	CharaEntity m_charaEntity;

	CharaHash m_pPlayerHashes;
	CharaHash m_pEnemyHashes;
};

#endif