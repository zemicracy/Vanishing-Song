#include "BattleBoss.h"
#include "ResourceManager.h"

using namespace aetherClass;

BattleBoss::BattleBoss()
{
}


BattleBoss::~BattleBoss()
{
	mFinalize();
}

void BattleBoss::mInitialize(eMusical type, aetherClass::ViewCamera* camera, aetherClass::Vector3& pos)
{
	m_pModel = std::make_shared<FbxModel>();
	m_pModel = ResourceManager::mGetInstance().mGetEnemyHash(type);

	m_pModel->SetCamera(camera);
	m_pModel->property._transform._translation = pos;
	m_pModel->property._transform._scale = 1.5;
	m_pModel->property._transform._rotation._y = -90;
}

void BattleBoss::mUpdate(const float timescale){
	

}

void BattleBoss::mRender(std::shared_ptr<ShaderBase> tex){
	m_pModel->Render(tex.get());
}

void BattleBoss::mFinalize(){
	if (m_pModel){
		m_pModel.reset();
	}
}