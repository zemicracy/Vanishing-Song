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

	// �S�̂̏�����
	bool Initialize();

	// �S�̂̉��
	void Finalize();
	
	// ��{BGM�𗬂��悤
	void PlayBaseBGM(const int id);

	// �e�N�X�`���擾�p
	std::shared_ptr<aetherClass::Texture> GetTexture(std::string);

	std::shared_ptr<ActionSound> GetActionSound(eCommandType);

	// ��{�I�Ɏg���V�F�[�_�[�̎擾�p
	std::unordered_map<std::string, std::shared_ptr<aetherClass::ShaderBase>>& mGetShaderHash();

	void mPlayerInitialize(eMusical,std::string directy);
	std::shared_ptr<GearFrame> mGetPlayerHash(eMusical);

	void mEnemyInitialize(eEnemyType, std::string directy);
	std::vector<std::shared_ptr<GearFrame>>& mGetEnemyHash(eEnemyType);
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
	/*
		�A�N�V�����R�}���h�ɑΉ��������̓o�^�p
	*/
	bool RegisterActionSound(eCommandType, std::string path);

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