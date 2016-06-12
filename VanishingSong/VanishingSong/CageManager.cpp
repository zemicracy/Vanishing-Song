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
		const auto position = enemyManager->mEnemyGet(count)->mGetProperty()._penemy->property._transform._translation;
		bool isTought = false;
		// 戦闘に使うプレイヤーはもう捕まっていないから檻を外す
		for (auto& useIndex : GameManager::mGetInstance().mGetUsePlayer()){
			if (musical[count] == useIndex.second){
				isTought = true;
			}
		}

		// 捕まっていたら
		if (isTought){
			index = std::make_shared<Cage>(ResourceManager::mGetInstance().mGetPlayerHash(musical[count]), Vector3(position._x + 20, 0.0f, position._z), camera, false);
		}
		else{
			index = std::make_shared<Cage>(ResourceManager::mGetInstance().mGetPlayerHash(musical[count]), Vector3(position._x + 20, 0.0f, position._z), camera, false);
		}
		count += 1;
	}

}


void CageManager::mRender(ShaderBase* tex, ShaderBase* color){
	// 捕虜の表示
	for (auto& index : m_pCage){
		index->mRender(tex, color);
	}

	return;
}


void CageManager::mUpdate(float scaleTime, Vector3 position){
	// 捕虜を向かせる
	for (auto& index : m_pCage){
		index->mUpdate(scaleTime,position);
	}
}