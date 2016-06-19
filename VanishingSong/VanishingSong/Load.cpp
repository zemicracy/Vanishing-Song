#include "Load.h"
#include <Rectangle2D.h>
#include "Const.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include <GameClock.h>
#include <WorldReader.h>
#include <GameController.h>
using namespace aetherClass;
namespace{
	const float kChangeBarTime = 0.3f;
}
Load::Load()
{
	m_changeBarTime = NULL;
	m_barCount = NULL;
}


Load::~Load()
{
	m_changeBarTime = NULL;
	m_barCount = NULL;
}

void Load::Initialize(){
	bool result;
	m_pLoadBar = std::make_unique<Rectangle2D>();
	m_pLoadBar->Initialize();

	WorldReader reader;
	reader.Load("data\\Load\\load_screen", true);
	for (auto& index : reader.GetInputWorldInfo()._object){
		if (index->_name == "now_loading"){
			m_pLoadBar->property._transform = index->_transform;
		}
	}

	m_model = std::make_shared<FbxModel>();
	m_model->LoadFBX("Model\\Load\\model.fbx", eAxisSystem::eAxisOpenGL);
	m_model->SetTextureDirectoryName("Model\\Load\\tex");
	m_model->SetCamera(&m_view);
	m_model->property._transform._scale._x = -1;
	m_model->property._transform._rotation._y = 180;
	m_model->property._transform._translation._y = -40;

	m_view.property._translation._z = -100;
}


void Load::Finalize(){
	if (m_pLoadBar){
		m_pLoadBar->Finalize();
		m_pLoadBar.reset();
		m_pLoadBar = nullptr;
	}

}

/*
	ƒ[ƒh’†‚Ìˆ—
*/
void Load::Run(){
	auto shader = ResourceManager::mGetInstance().mGetShaderHash();
	m_view.Render();
	m_model->KeyframeUpdate(m_model->GetKeyframeNameList(0),true);
	m_model->KeyframeAnimationRender(shader["texture"].get());

	mChangeBar(m_changeBarTime);
	DirectXEntity entity;
	entity.GetDirect3DManager()->Change2DMode();
	m_pLoadBar->Render(shader["texture"].get());
	entity.GetDirect3DManager()->Change3DMode();
	
	m_changeBarTime += GameClock::GetDeltaTime();
	
	auto loadBarTexture = ResourceManager::mGetInstance().GetTexture("NowLoading" + std::to_string(m_barCount + 1)).get();
	m_pLoadBar->SetTexture(loadBarTexture);
	
}

bool Load::WaitRun(){
	Run();
	DirectXEntity entity;
	entity.GetDirect3DManager()->Change2DMode();
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		entity.GetDirect3DManager()->Change3DMode();
		return kWaitting;
	}
	entity.GetDirect3DManager()->Change3DMode();
	return kWaitEnd;

}

void Load::mChangeBar(float& time){
	if (time > kChangeBarTime){
		m_barCount += 1;
		if (m_barCount >= 6){
			m_barCount = NULL;
		}
		auto loadBarTexture = ResourceManager::mGetInstance().GetTexture("NowLoading" + std::to_string(m_barCount+1)).get();
		m_pLoadBar->SetTexture(loadBarTexture);
		time = NULL;
	}
}

// •Ï”‚È‚Ç‚ÌÄ‰Šú‰»
void Load::mResetProperty(){
	m_changeBarTime = NULL;
	m_barCount = NULL;
}