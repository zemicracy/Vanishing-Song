#include "CageManager.h"
#include "ResourceManager.h"
#include "GameManager.h"

using namespace aetherClass;
CageManager::CageManager()
{
}


CageManager::~CageManager()
{
	if (!m_pCage.empty()){
		for (auto& index : m_pCage){
			if (!index)continue;
			index.reset();
		}
	}
}


void CageManager::mInitialize(FieldEnemyManager* enemyManager, ViewCamera* camera){
	int count = 0;
	eMusical musical[3] = { eMusical::eGreen, eMusical::eRed, eMusical::eYellow };

	for (auto& index : m_pCage){
		bool isThought = true;
		for (auto& usePlayer : GameManager::mGetInstance().mGetUsePlayer()){
			if (musical[count] == usePlayer.second){
				isThought = false;
				break;
			}
		}
		const auto position = enemyManager->mEnemyGet(count)->mGetProperty()._pEnemy->property._transform._translation;
		index = std::make_shared<Cage>(ResourceManager::mGetInstance().mGetPlayerHash(musical[count]), Vector3(position._x + 50, 0.0f, position._z), camera, isThought);

		index->mSetMessagePath(0, "Texture\\Message\\FieldPlayerCage\\" + std::to_string(count) + ".png");
		index->mSetMessagePath(1, "Texture\\Message\\FieldPlayer\\" + std::to_string(count) + ".png");
		index->mRegisterIcon(mGetIconPath(musical[count]));
		count += 1;
	}
}


void CageManager::mRender(ShaderBase* tex, ShaderBase* color){
	// �ߗ��̕\��
	for (auto& index : m_pCage){
		index->mRender(tex, color);
	}

	return;
}

//
void CageManager::mUpdate(float scaleTime, Vector3 position, const bool button){
	// �ߗ�����������
	for (auto& index : m_pCage){
		index->mUpdate(scaleTime,position,button);
	}
}

//
void CageManager::mUIRender(aetherClass::ShaderBase* tex){
	for (auto& index : m_pCage){
		index->mUIRender(tex);
	}
}

//
Vector3 CageManager::mGetPosition(const int number){
	if (number >= m_pCage.size())return kVector3Zero;
	return m_pCage.at(number)->mGetPosition();
}

//
void CageManager::mSetIsComment(const int number, const bool flg){
	if (number >= m_pCage.size())return;
	m_pCage.at(number)->mSetIsComment(flg);
}

//
bool CageManager::mGetIsMessageRun(const int number){
	if (number >= m_pCage.size())return false;
	return m_pCage.at(number)->mGetMessageRun();
}

std::shared_ptr<aetherClass::Sphere>& CageManager::mGetColldier(const int id){
	return m_pCage.at(id)->mGetCollider();
}

std::string CageManager::mGetIconPath(eMusical type){
	switch (type)
	{
	case eMusical::eRed:
		return "Texture\\Icon\\Player\\red.png";
	case eMusical::eYellow:
		return "Texture\\Icon\\Player\\yellow.png";
	case eMusical::eGreen:
		return "Texture\\Icon\\Player\\green.png";
	default:
		break;
	}
}