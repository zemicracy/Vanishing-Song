#pragma once

#include"GaugeBase.h"
#include"CharaStatus.h"
class ClearGauge : public GaugeBase
{
public:
	ClearGauge();
	~ClearGauge();

	bool mInitialize()override;
	void mRender(std::shared_ptr<HalfFillShader>)override;
	void mUpdate(float)override;
	
	
	//AccessorMethods
	void mSetTransform(aetherClass::Transform);
	void mSetRate(float);

private:
	void mFinalize()override;

	aetherClass::Transform m_trans;
	HalfFillShader::DirectionGaugeType m_direcType;

	struct UseValue{
		float _interpolation;
		UseValue(){
			SecureZeroMemory(this, sizeof(UseValue));
		}
	}m_useValue;

};

