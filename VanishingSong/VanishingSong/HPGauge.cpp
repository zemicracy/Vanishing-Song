#include "HPGauge.h"
#include"Rectangle2D.h"
#include"GameController.h"

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
	if (m_shader){
		m_shader->Finalize();
		m_shader.reset();
	}
}
bool HPGauge::mInitialize(){
	m_sprite = std::make_shared<Rectangle2D>();
	m_sprite->Initialize();

	m_sprite->property._transform._scale._x = 300;
	m_sprite->property._transform._scale._y = 300;
	m_sprite->property._transform._translation._x = 500;
	m_sprite->property._transform._translation._y = 340;
//	m_sprite->property._transform._rotation._z = 90;

	m_sprite->property._color = Color(1, 1, 0, 1);
	
	m_texture = std::make_shared<Texture>();
	m_texture->Load("Texture/space.jpg");
	m_sprite->SetTexture(m_texture.get());

	ShaderDesc desc;
	desc._pixel._srcFile = L"Shader/HalfFiller.hlsl";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._vertex._entryName = "vs_main";

	m_shader = std::make_shared<HalfFillShader>();
	m_shader->Initialize(desc,ShaderType::ePixel | ShaderType::eVertex);
	m_shader->_property._flg = 1;
	m_shader->_property._beginRadius = 0;
	m_shader->_property._endRadius = 220;
	return true;
}

void HPGauge::mUpdate(float timeScale){
//	static float x = 0, y = 0;
	m_shader->_property._position = Vector2(0.4, 0.6);
	if (GameController::GetKey().IsKeyDown(VK_LEFT)){
		m_shader->_property._interpolation -= 0.01;
	}
	else if (GameController::GetKey().IsKeyDown(VK_RIGHT)){
		m_shader->_property._interpolation += 0.01;
	}
	/*if (GameController::GetKey().IsKeyDown(VK_LEFT)){
		x -= 0.01;
	}
	else if (GameController::GetKey().IsKeyDown(VK_RIGHT)){
		x += 0.01;
	}
	if (GameController::GetKey().IsKeyDown(VK_UP)){
		y -= 0.01;
	}
	else if (GameController::GetKey().IsKeyDown(VK_DOWN)){
		y += 0.01;
	}*/

	//printf("Tan	\t%.2f\n x\t%.2f,y\t%.2f\n", 180+atan2(y, x)*180/3.14,x,y);
	//printf("Begin\t%.2f\n",0* 0.0174532925f);

}

void HPGauge::mRender(ShaderBase *shader){
	m_sprite->Render(m_shader.get());
}
