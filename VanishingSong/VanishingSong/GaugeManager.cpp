#include "GaugeManager.h"
#include"WorldReader.h"

using namespace aetherClass;
GaugeManager::GaugeManager()
{
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
	WorldReader reader;
	reader.Load("data\\gaugePosition.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "playerHP"){
			m_playerHpGauge = std::make_unique<PlayerHPGauge>();
			SpriteBase::Property pp;
			pp._transform = itr->_transform;
			pp._color = Color(0,1,0,1);

			m_playerHpGauge->mSetProperty(pp);
			m_playerHpGauge->mInitialize();

		}
		else if (itr->_name == "enemyHP"){
			m_enemyHpGauge = std::make_unique<PlayerHPGauge>();
			SpriteBase::Property pp;
			pp._transform = itr->_transform;
			pp._color = Color(1,0.5,0,1);

			m_enemyHpGauge->mSetProperty(pp);
			m_enemyHpGauge->mInitialize();
		}

	}


	ShaderDesc desc;
	desc._pixel._srcFile = L"Shader/HalfFiller.hlsl";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._vertex._entryName = "vs_main";

	m_shader = std::make_shared<HalfFillShader>();
	m_shader->Initialize(desc, eVertex | ePixel);
	
	return true;
}
void GaugeManager::mRender(){
	m_playerHpGauge->mRender(m_shader);
	m_enemyHpGauge->mRender(m_shader);
}
void GaugeManager::mUpdate(float timeScale){
	m_playerHpGauge->mUpdate(timeScale);
	m_enemyHpGauge->mUpdate(timeScale);
}


void GaugeManager::mSetHpAll(CharaStatus* player, CharaStatus* enemy){
	m_playerHpGauge->mSetCharaStatus(player);
	m_enemyHpGauge->mSetCharaStatus(enemy);
}