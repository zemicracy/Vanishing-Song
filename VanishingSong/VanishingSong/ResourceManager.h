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

	// �S�̂̏�����
	bool Initialize();

	// �S�̂̉��
	void Finalize();
	
	// ��{BGM�𗬂��悤
	void PlayBaseBGM(const int id);

	// �e�N�X�`���擾�p
	std::shared_ptr<aetherClass::Texture> GetTexture(std::string);

	std::shared_ptr<ActionSound> GetActionSound(eCommandType);

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
		�A�N�V�����R�}���h�ɑΉ��������̓o�^�p
	*/
	bool RegisterActionSound(eCommandType, std::string path);

	/*
		�e�N�X�`���̓o�^�p
	*/
	bool RegisterTexture(std::string registerName,std::string path);
private:
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>> m_pTextureHash;
	std::unordered_map<eCommandType, std::shared_ptr<ActionSound>> m_pActionSoundHash;
	std::array<std::shared_ptr<aetherClass::GameSound>,kMaxBGM> m_pBaseBgmArray;
	static std::string m_BgmPath[kMaxBGM];
};

#endif