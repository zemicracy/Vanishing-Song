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
		const auto position = enemyManager->mEnemyGet(count)->mGetProperty()._penemy->property._transform._translation;
		index = std::make_shared<Cage>(ResourceManager::mGetInstance().mGetPlayerHash(musical[count]), Vector3(position._x + 30, 0.0f, position._z), camera, isThought);
		count += 1;
	}
}


void CageManager::mRender(ShaderBase* tex, ShaderBase* color){
	// •ß—¸‚Ì•\Ž¦
	for (auto& index : m_pCage){
		index->mRender(tex, color);
	}

	return;
}


void CageManager::mUpdate(float scaleTime, Vector3 position){
	// •ß—¸‚ðŒü‚©‚¹‚é
	for (auto& index : m_pCage){
		index->mUpdate(scaleTime,position);
	}
}