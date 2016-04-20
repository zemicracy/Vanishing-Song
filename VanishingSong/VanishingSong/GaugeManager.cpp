#include "GaugeManager.h"

using namespace aetherClass;
GaugeManager::GaugeManager()
{
}


GaugeManager::~GaugeManager(){
	mFinalize();
}

void GaugeManager::mFinalize(){
	m_shader->Finalize();
}


bool GaugeManager::mInitialize(){
	bool result = false;
	m_hpGauge = std::make_unique<HPGauge>();
	result = m_hpGauge->mInitialize();
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
	m_hpGauge->mRender(m_shader.get());
}
void GaugeManager::mUpdate(float timeScale){
	m_hpGauge->mUpdate(timeScale);
}
