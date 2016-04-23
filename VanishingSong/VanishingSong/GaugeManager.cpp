#include "GaugeManager.h"
#include<PixelShader.h>

using namespace aetherClass;
GaugeManager::GaugeManager()
{
	m_hpGauge = nullptr;
	m_mpGauge= nullptr;
	m_shader = nullptr;
}


GaugeManager::~GaugeManager(){
	mFinalize();
}

void GaugeManager::mFinalize(){
	if (m_shader){
		m_shader->Finalize();
	}
}


bool GaugeManager::mInitialize(){
	bool result = false;
	m_hpGauge = std::make_unique<PlayerHPGauge>();
	result = m_hpGauge->mInitialize();
	if (!result)return false;

	m_mpGauge = std::make_unique<PlayerMPGauge>();
	result = m_mpGauge->mInitialize();
	if (!result)return false;

	ShaderDesc desc;
	desc._pixel._srcFile = L"Shader/HalfFiller.hlsl";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._vertex._entryName = "vs_main";

	m_shader = std::make_shared<HalfFillShader>();
	result = m_shader->Initialize(desc, eVertex | ePixel);
	if (!result) return false;

	return true;
}
void GaugeManager::mRender(){
	m_hpGauge->mRender(m_shader);
	m_mpGauge->mRender(m_shader);
}
void GaugeManager::mUpdate(float timeScale){
	m_mpGauge->mUpdate(timeScale);
	m_hpGauge->mUpdate(timeScale);
}

void GaugeManager::mSetuseMp(float value){
	m_mpGauge->mSetuseMpValue(value);
}

void GaugeManager::mSetCharaStatus(CharaStatus *status){
	m_mpGauge->mSetCharaStatus(status);
	m_hpGauge->mSetCharaStatus(status);
}

//m_pCharaStatus = status;
