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
	BGM�̃p�X���z��
	BGM��ς������ꍇ�͂������������Ă�
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
	���\�[�X�n�̏���������
*/
bool ResourceManager::Initialize(){
	Finalize();

	InitializeBGM();
	InitializeActionSound();
	InitializeTexture();
	return true;
}

/*
	���\�[�X�n�̉������
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
	��{BGM�𗬂��z

*/
void ResourceManager::PlayBaseBGM(const int id){
	m_pBaseBgmArray[id]->PlayToLoop();
	return;
}

/*
	�e�N�X�`���̎擾�p
*/
std::shared_ptr<aetherClass::Texture> ResourceManager::GetTexture(std::string name){
	return m_pTextureHash[name];
}

/*
��{BGM�̏�����

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
			Debug::mErrorPrint("BGM�̓ǂݍ��݂Ɏ��s���܂���", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		}
	}

	return true;
}


/*
	�A�N�V�����R�}���h�ɑΉ����鉹�̏�����
*/
bool ResourceManager::InitializeActionSound(){
	RegisterActionSound(eActionType::eShortDistanceAttack, "Sound/damage03.wav");
	return true;
}


/*
	�e�N�X�`���̏�����
*/
bool ResourceManager::InitializeTexture(){
	return true;
}

/*
BGM�̔z��̗v�f���폜
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
	�A�N�V�����R�}���h�ɑΉ�����A�z�z��̉������
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
	�e�N�X�`���̔z����폜
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
	�A�N�V�����R�}���h�ɑΉ������T�E���h�̓o�^	
*/
bool ResourceManager::RegisterActionSound(eActionType type, std::string path){
	auto findMap = m_pActionSoundHash.find(type);

	// ���łɂ��̖��O�œo�^���Ă���̂ł���Ή������Ȃ�
	if (findMap != m_pActionSoundHash.end())
	{
		Debug::mErrorPrint("���ɓo�^�ς݂̃L�[�̂��ߓo�^���ł��܂���ł���", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		return false;
	}

	std::shared_ptr<ActionSound> action;
	action = std::make_shared<ActionSound>();
	action->mLoad(path);
	m_pActionSoundHash.insert(std::make_pair(type, action));
	return true;
}

/*
	�e�N�X�`���̓o�^�p
*/
bool ResourceManager::RegisterTexture(std::string registerName, std::string path){
	
	auto findMap = m_pTextureHash.find(registerName);

	// ���łɂ��̖��O�œo�^���Ă���̂ł���Ή������Ȃ�
	if (findMap != m_pTextureHash.end())
	{
		Debug::mErrorPrint("���ɓo�^�ς݂̃L�[�̂��ߓo�^���ł��܂���ł���", __FILE__, __FUNCTION__, __LINE__, Debug::eState::eConsole);
		return false;
	}

	// �o�^����
	std::shared_ptr<Texture> texture;
	texture->Load(path);
	m_pTextureHash.insert(std::make_pair(registerName, texture));

	return true;
}
