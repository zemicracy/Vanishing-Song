#include "FieldEnemyManager.h"
#include "FieldEnemy.h"
#include <random>
#include "PixelShader.h"
#include <chrono>
#include "WorldReader.h"
#include "Singleton.h"
#include "ResourceManager.h"

using namespace aetherClass;
FieldEnemyManager::FieldEnemyManager()
{
}

FieldEnemyManager::~FieldEnemyManager()
{
}

bool FieldEnemyManager::mInitilize(aetherClass::ViewCamera* camera){
	
	EnemySize = 1;

	WorldReader reader;
	reader.Load("data\\stage.aether");

	//Spawn�ꏊ
	for (auto index : reader.GetInputWorldInfo()._object){

		if (index->_name == "area1"){
			m_pEnemySpawner[0] = index->_transform._translation;
		}
		if (index->_name == "area2"){
			m_pEnemySpawner[1] = index->_transform._translation;
		}
		if (index->_name == "area3"){
			m_pEnemySpawner[2] = index->_transform._translation;
		}
		if (index->_name == "area4"){
			m_pEnemySpawner[3] = index->_transform._translation;
		}
	}

	reader.UnLoad();


	//EnemyGround�̐���
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.insert(m_pEnemy.begin(),std::make_shared<FieldEnemy>());
		m_pEnemy.begin()->get()->mInitialize(FieldEnemy::eType::Ground,camera);
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");

	}

	//EnemyAir�̐���
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<FieldEnemy>());
		m_pEnemy.begin()->get()->mInitialize(FieldEnemy::eType::Air, camera);
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
	}

	//Enemy(��)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<FieldEnemy>());
		m_pEnemy.begin()->get()->mInitialize(FieldEnemy::eType::Blue, camera);
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
	}

	//Enemy(��)
	for (int i = 0; i < EnemySize; i++){
		m_pEnemy.insert(m_pEnemy.begin(), std::make_shared<FieldEnemy>());
		m_pEnemy.begin()->get()->mInitialize(FieldEnemy::eType::Yellow, camera);
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
		m_pEnemy.begin()->get()->mRegisterMessage("Texture\\Message\\tmplate.png");
	}

	//�G�̏����ʒu
	mSetPosion();

	m_isRender = false;
	m_isJudge = false;
	return true;
}


//�`�揈��
void FieldEnemyManager::mRender(aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){
	
		for (int i = 0; i < m_pEnemy.size(); i++){
			m_pEnemy[i]->mRender(model_shader, colider_shader);
		}
}

bool FieldEnemyManager::mGetIsJudge(){

	return m_isJudge;
}

//�X�V����
void FieldEnemyManager::mUpdater(){

	for (auto itr : m_pEnemy){
	itr->mUpdate();
	}

}


//�������(�R���C�_�[�͂��Ȃ��Ă���)
void FieldEnemyManager::mFinalize(){
	
	for (auto itr:m_pEnemy){
	itr->mFinalize();
	}

}

void FieldEnemyManager::mSetPosion(){

	//�G�̏o���ʒu
	for (int i = 0; i <m_pEnemy.size(); i++){
		m_pEnemy[i]->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation = m_pEnemySpawner[i];
		m_pEnemy[i]->mGetProperty()._penemy->m_pBody->_pGear->property._transform._translation._y = +20;
		m_pEnemy[i]->mGetProperty()._enemyAreaNo = i;
		m_enemyArray[i]=m_pEnemy[i];
	}
}

std::shared_ptr<FieldEnemy> FieldEnemyManager::mEnemyGet(int enemy){
	return  m_enemyArray[enemy];
}
