#include "HPGauge.h"
#include"Rectangle2D.h"
#include"GameController.h"
#include<WorldReader.h>


using namespace aetherClass;
HPGauge::HPGauge()
{
}


HPGauge::~HPGauge()
{
	mFinalize();
}


void HPGauge::mFinalize(){
	if (m_sprite){
		m_sprite->Finalize();
		m_sprite.reset();
	}
	if (m_maskTexture){
		m_maskTexture.reset();
	}
}
bool HPGauge::mInitialize(){
	bool result = false;
	
	m_sprite = std::make_shared<Rectangle2D>();
	result = m_sprite->Initialize();
	if (!result)return false;

	m_maskTexture = std::make_shared<Texture>();
	result = m_maskTexture->Load("Texture/hpGauge.png");
	if (!result)return false;

	WorldReader reader;
	reader.Load("data/gaugePosition.aether");
	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "hp_gauge"){
			m_sprite->property._transform = itr->_transform;
			m_sprite->property._color = itr->_color;
		}
	}
	reader.UnLoad();

	//“h‚è•û‚ÌŽw’è
	m_fillType._flg = 0;
	m_fillType._direction = Vector2(0, 1);
	m_fillType._interpolation = 0;

	return true;
}

void HPGauge::mUpdate(float timeScale){
	if (GameController::GetKey().IsKeyDown(VK_LEFT)){
		m_fillType._interpolation -= 0.01;
	}
	else if (GameController::GetKey().IsKeyDown(VK_RIGHT)){
		m_fillType._interpolation += 0.01;
	}
}

void HPGauge::mRender(ShaderBase *shader){
	static_cast<HalfFillShader*>(shader)->_property = m_fillType;
	m_sprite->Render(shader);
}
