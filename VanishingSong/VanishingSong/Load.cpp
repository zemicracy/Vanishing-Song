#include "Load.h"
#include <Rectangle2D.h>
#include "Const.h"
#include "ResourceManager.h"
using namespace aetherClass;

Load::Load()
{
}


Load::~Load()
{
}

void Load::Initialize(){
	bool result;
	m_pLoadMainTexture = std::make_unique<Texture>();
	result = m_pLoadMainTexture->Load("Texture\\Load\\LaodMain.png");
	if (!result){
		Debug::mPrint("テクスチャの読み込み失敗");
	}
	m_pLoadMain = std::make_unique<Rectangle2D>();
	m_pLoadMain->Initialize();
	m_pLoadMain->property._transform._scale = Vector3(kWindowWidth, kWindowHeight, 0);
	m_pLoadMain->SetTexture(m_pLoadMainTexture.get());
}


void Load::Finalize(){
	if (m_pLoadMain){
		m_pLoadMain->Finalize();
		m_pLoadMain.release();
		m_pLoadMain = nullptr;
	}

	if (m_pLoadMainTexture){
		m_pLoadMainTexture.release();
		m_pLoadMainTexture = nullptr;
	}
}

/*
	ロード中の処理
*/
void Load::Run(){
	DirectXEntity entity;
	entity.GetDirect3DManager()->Change2DMode();

	auto shader = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pLoadMain->Render(shader["texture"].get());
	entity.GetDirect3DManager()->Change3DMode();

}
bool Load::WaitRun(){
	return kWaitEnd;
}