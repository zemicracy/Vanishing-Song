#pragma once

#include<memory>
#include"HalfFillShader.h"
#include"SpriteBase.h"
class HPGauge
{
public:
	HPGauge();
	~HPGauge();

	bool mInitialize();
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(float);
private:
	void mFinalize();

	std::shared_ptr<aetherClass::SpriteBase>m_sprite;
	std::shared_ptr<HalfFillShader>m_shader;
	std::shared_ptr<aetherClass::Texture>m_texture;
};

