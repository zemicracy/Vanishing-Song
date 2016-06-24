#ifndef LOAD_H
#define LOAD_H
#include <LoadScreen.h>
#include <memory>
#include <SpriteBase.h>
#include <FbxModel.h>
#include "HalfFillShader.h"
class Load :
	public aetherClass::LoadScreen
{
public:
	Load();
	~Load();

	void Initialize()override;
	void Finalize()override;
	void Run()override;
	bool WaitRun()override;

private:
	void mChangeBar(float& time);
	void mResetProperty();
	void mUIRender(aetherClass::ShaderBase*,std::shared_ptr<HalfFillShader>&);
private:
	std::unique_ptr<aetherClass::SpriteBase> m_pLoadBar;
	std::shared_ptr<aetherClass::SpriteBase> m_pWave;
	std::shared_ptr<aetherClass::SpriteBase> m_pWaveMask;
	std::shared_ptr<HalfFillShader> m_pWaveShader;
	aetherClass::Texture m_waveTexture;
	aetherClass::Texture m_waveTextureMask;

	std::shared_ptr<aetherClass::FbxModel> m_model;

	float m_changeBarTime;
	int m_barCount;
	aetherClass::ViewCamera m_view;

	HalfFillShader::DirectionGaugeType m_direcType;
	float m_originInterpolation;
	float m_maskInterpolation;
	bool m_changeDirection;
};

#endif