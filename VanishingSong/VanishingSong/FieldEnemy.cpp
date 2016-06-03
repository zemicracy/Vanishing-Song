#include "FieldEnemy.h"
#include <WorldReader.h>
#include "ResourceManager.h"
#include <Singleton.h>
#include "Debug.h"

using namespace aetherClass;

FieldEnemy::FieldEnemy(){
	m_message.clear();
}

FieldEnemy::~FieldEnemy()
{
	m_message.clear();
}

bool FieldEnemy::mInitialize(eType type, ViewCamera* camera, std::string dataPath){
	m_dataPath = dataPath;
	switch (type)
	{
	case eType::Ground:
		mInitializeGround(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Air:
		mInitializeAir(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Blue:
		mInitializeBlue(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Yellow:
		mInitializeYellow(camera);
		mInitializeEnemyColider(camera);
		break;
	case eType::Null:

	default:
		break;
	}

	return true;
}

//Enemy�n��
bool FieldEnemy::mInitializeGround(ViewCamera* camera){

		m_property._penemy = std::make_shared<GearFrame>();
	
		auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eRed, eEnemyType::eGround);

		m_property._penemy = gearframe;

		m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;

		// �ŏ�ʂɓ�����p�[�c�̐ݒ�
		m_pTopGear = m_property._penemy->m_pBody;

		// �̂Ƀp�[�c�Ƃ̐e�q�֌W
		m_charaEntity.SetCamera(m_pTopGear,camera);

		return true;
}

//Enemy��
bool FieldEnemy::mInitializeAir(ViewCamera* camera){

	m_property._penemy = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eGreen, eEnemyType::eAir);
	m_property._penemy = gearframe;
	
	m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;

	// �ŏ�ʂɓ�����p�[�c�̐ݒ�
	m_pTopGear = m_property._penemy->m_pBody;

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	m_charaEntity.SetCamera(m_pTopGear, camera);

	return true;

}

//���p�G
bool FieldEnemy::mInitializeBlue(ViewCamera* camera){

	m_property._penemy = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eBlue,eEnemyType::eAir);

	m_property._penemy = gearframe;

	m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;

	// �ŏ�ʂɓ�����p�[�c�̐ݒ�
	m_pTopGear = m_property._penemy->m_pBody;

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	m_charaEntity.SetCamera(m_pTopGear, camera);

	return true;
}

//���p�G
bool FieldEnemy::mInitializeYellow(ViewCamera* camera){

	m_property._penemy = std::make_shared<GearFrame>();

	auto gearframe = Singleton<ResourceManager>::GetInstance().mGetEnemyHash(eMusical::eYellow, eEnemyType::eAir);

	m_property._penemy = gearframe;

	m_property._penemy->m_pBody->_pGear->property._transform._scale = 2;

	// �ŏ�ʂɓ�����p�[�c�̐ݒ�
	m_pTopGear = m_property._penemy->m_pBody;

	m_charaEntity.SetCamera(m_pTopGear, camera);

	return true;
}


//�R���C�_�[�̏�����
void FieldEnemy::mInitializeEnemyColider(ViewCamera* camera){
	m_property._pCollider = std::make_shared<Cube>();
	m_property._pCollider->Initialize();
	m_property._pCollider->property._transform._translation = m_pTopGear->_pGear->property._transform._translation;
	m_property._pCollider->property._transform._scale = Vector3(9, 12, 4);
	m_property._pCollider->property._color = Color(1, 1, 1, 0.5);
	m_property._pCollider->SetCamera(camera);

}

//�X�V����
void FieldEnemy::mUpdate(){
	m_property._pCollider->property._transform._translation = m_property._penemy->m_pBody->_pGear->property._transform._translation;
}

void FieldEnemy::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	m_charaEntity.mGearRender(m_pTopGear, model_shader, colider_shader);

	m_property._pCollider->Render(colider_shader);
}


FieldEnemy::Property& FieldEnemy::mGetProperty(){
	return m_property;
}

void FieldEnemy::mEnemyOnHit(){
	
}
void FieldEnemy::mFaceToPlayer(aetherClass::Vector3 position){
	m_charaEntity.mFaceToObject(m_pTopGear, position,"=");
}
void FieldEnemy::mFinalize(){

	if (m_property._pCollider){
		m_property._pCollider->Finalize();
		m_property._pCollider.reset();
		m_property._pCollider = nullptr;
	}

	for (auto index : m_message){
		index.reset();
		index = nullptr;
	}
	m_message.clear();
}


//�@�o�^�p
void FieldEnemy::mRegisterMessage(std::string path){
	m_message.resize(m_message.size() + 1);
	const int id = m_message.size() - 1;
	m_message[id] = std::make_shared<Texture>();
	m_message[id]->Load(path);
}
int FieldEnemy::mGetMessageNum()const{
	return m_message.size();
}

std::shared_ptr<aetherClass::Texture> FieldEnemy::mGetMessage(const int id){
	return m_message[id];
}

std::string FieldEnemy::mGetBattleDataPath(){
	return m_dataPath;
}