#pragma once
#include"HalfFillShader.h"
#include<memory>
#include<SpriteBase.h>
class GaugeBase
{
public:

	GaugeBase(){
		m_pMainSprite = nullptr;
		m_pMaskSprite = nullptr;
		m_maskTexture = nullptr;
		m_maxGaugeValue = 0;
		m_maxInterPolate = 0;
	}
	virtual ~GaugeBase() = default;

	virtual bool mInitialize(){ return true; }
	virtual void mUpdate(float timeScale){};
	virtual void mRender(std::shared_ptr<HalfFillShader> shader){};
protected:
	virtual void mFinalize(){};

	std::shared_ptr<aetherClass::SpriteBase>m_pMaskSprite;
	std::shared_ptr<aetherClass::SpriteBase>m_pMainSprite;

	std::shared_ptr<aetherClass::Texture>m_maskTexture;
	

	//�h����ŕK�v�Ȍ^�����錾���Ă��������B
	//HalfFillShader::FillType m_fillType;
	//HalfFillShader::DirectionGaugeType m_fillType;

	float m_maxGaugeValue;	//�Q�[�W�̍ő�l
	float m_maxInterPolate;	//�����l�̍ő�l

};