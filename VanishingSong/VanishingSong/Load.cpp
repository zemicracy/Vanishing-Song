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
	const float kWaveMaxValue = 0.45f;
}

Load::Load()
{
	m_changeBarTime = NULL;
	m_barCount = NULL;
}

//
Load::~Load()
{
	m_changeBarTime = NULL;
	m_barCount = NULL;
}

//
void Load::Initialize(){
	m_pLoadBar = std::make_unique<Rectangle2D>();
	m_pLoadBar->Initialize();

	WorldReader reader;
	reader.Load("data\\Load\\load_screen", true);
	for (auto& index : reader.GetInputWorldInfo()._object){
		if (index->_name == "now_loading"){
			m_pLoadBar->property._transform = index->_transform;
		}
	}

	//
	m_model = std::make_shared<FbxModel>();
	m_model->LoadFBX("Model\\Load\\model.fbx", eAxisSystem::eAxisOpenGL);
	m_model->SetTextureDirectoryName("Model\\Load\\tex");
	m_model->SetCamera(&m_view);
	m_model->property._transform._scale= Vector3(-0.5, 0.5, 0.5);
	m_model->property._transform._rotation._y = 180;
	m_model->property._transform._translation._y = -40;
	m_model->property._transform._translation._x = 40;
	m_view.property._translation._z = -100;

	// メイン
	m_waveTexture.Load("Texture\\Load\\wave.png");
	m_pWave = std::make_shared<Rectangle2D>();
	m_pWave->Initialize();
	m_pWave->property._transform._scale = Vector3(kWindowWidth, kWindowHeight, 0);
	m_pWave->SetTexture(&m_waveTexture);

	// マスク
	m_waveTextureMask.Load("Texture\\Load\\wave_mask.png");
	m_pWaveMask = std::make_shared<Rectangle2D>();
	m_pWaveMask->Initialize();
	m_pWaveMask->property._transform._scale = Vector3(kWindowWidth, kWindowHeight, 0);
	m_pWaveMask->SetTexture(&m_waveTextureMask);

	// 
	ShaderDesc desc;
	desc._pixel._srcFile = L"Shader/HalfFiller.hlsl";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._vertex._entryName = "vs_main";

	m_pWaveShader = std::make_shared<HalfFillShader>();
	m_pWaveShader->Initialize(desc, eVertex | ePixel);

	//
	m_direcType._direction = Vector2(1, 0);
	m_direcType._interpolation = 1;
	m_pWaveShader->_property = m_direcType.mCastFillType();
	m_originInterpolation = NULL;
	m_maskInterpolation = NULL;
	m_changeDirection = false;

	return;
}

//
void Load::Finalize(){
	
	//
	if (m_pLoadBar){
		m_pLoadBar->Finalize();
		m_pLoadBar.reset();
		m_pLoadBar = nullptr;
	}

	//
	if (m_pWave){
		m_pWave->Finalize();
		m_pWave.reset();
		m_pWave = nullptr;
	}

	//
	if (m_pWaveMask){
		m_pWaveMask->Finalize();
		m_pWaveMask.reset();
		m_pWaveMask = nullptr;
	}

	//
	if (m_pWaveShader){
		m_pWaveShader->Finalize();
		m_pWaveShader.reset();
		m_pWaveShader = nullptr;
	}
}

/*
	ロード中の処理
*/
void Load::Run(){
	auto shader = ResourceManager::mGetInstance().mGetShaderHash();
	m_view.Render();
	m_model->KeyframeUpdate(m_model->GetKeyframeNameList(0),true);
	m_model->KeyframeAnimationRender(shader["texture"].get());

	mChangeBar(m_changeBarTime);
	
	DirectXEntity entity;
	entity.GetDirect3DManager()->Change2DMode();

	mUIRender(shader["texture"].get(), m_pWaveShader);
	
	entity.GetDirect3DManager()->Change3DMode();
	
	m_changeBarTime += GameClock::GetDeltaTime();
	
	auto loadBarTexture = ResourceManager::mGetInstance().GetTexture("NowLoading" + std::to_string(m_barCount + 1)).get();
	m_pLoadBar->SetTexture(loadBarTexture);
	
}

//
bool Load::WaitRun(){
	Run();
	DirectXEntity entity;
	entity.GetDirect3DManager()->Change2DMode();
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		entity.GetDirect3DManager()->Change3DMode();
		return kWaitting;
	}
	entity.GetDirect3DManager()->Change3DMode();
	mResetProperty();
	return kWaitEnd;

}

//
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

// 変数などの再初期化
void Load::mResetProperty(){
	m_changeBarTime = NULL;
	m_barCount = NULL;
	m_originInterpolation = NULL;
	m_maskInterpolation = NULL;
	m_changeDirection = false; 
	return;
}

//
void Load::mUIRender(ShaderBase* texture, std::shared_ptr<HalfFillShader>& shader){
	m_pLoadBar->Render(texture);
	if (m_changeDirection){
		m_originInterpolation += GameClock::GetDeltaTime();
		m_maskInterpolation = kWaveMaxValue;
	}
	else{
		m_maskInterpolation += GameClock::GetDeltaTime();
		m_originInterpolation = kWaveMaxValue;
	}
	

	// 状態によって描画順を変える
	if (m_changeDirection){
		m_pWaveShader->_property._interpolation = m_maskInterpolation;
		m_pWaveMask->Render(shader.get());

		m_pWaveShader->_property._interpolation = m_originInterpolation;
		m_pWave->Render(shader.get());
	}
	else{

		m_pWaveShader->_property._interpolation = m_originInterpolation;
		m_pWave->Render(shader.get());

		m_pWaveShader->_property._interpolation = m_maskInterpolation;
		m_pWaveMask->Render(shader.get());
	}
	

	if (m_maskInterpolation > kWaveMaxValue || m_originInterpolation > kWaveMaxValue){
		m_originInterpolation = NULL;
		m_maskInterpolation = NULL;
		m_changeDirection = !m_changeDirection;
	}
	return;
}