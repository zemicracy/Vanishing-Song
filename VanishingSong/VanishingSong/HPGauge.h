#pragma once

#include<memory>
#include"SpriteBase.h"
#include"HalfFillShader.h"
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
	std::shared_ptr<aetherClass::Texture>m_maskTexture;
	HalfFillShader::FillType m_fillType;

};

