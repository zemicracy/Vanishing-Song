#pragma once

#include"GaugeBase.h"
#include"CharaStatus.h"
class PlayerMPGauge : public GaugeBase
{
public:
	PlayerMPGauge();
	~PlayerMPGauge();

	bool mInitialize()override;
	void mRender(std::shared_ptr<HalfFillShader>)override;
	void mUpdate(float)override;

	//AccessorMethods
	void mSetCharaStatus(CharaStatus*);
	void mSetuseMpValue(float value);

private:
	void mFinalize()override;
	void mException();

	std::shared_ptr<aetherClass::SpriteBase>m_pIfuseSprite;
	HalfFillShader::DirectionGaugeType m_direcType;
	CharaStatus *m_CharaStatus;

	struct UseValue{
		float _interpolation;
		float _intervalTime;
		float _prevMp,_morePrevMp;
		float _nextMp,_prevNextMp;
		UseValue(){
			SecureZeroMemory(this, sizeof(UseValue));
		}
	}m_useValue;

	aetherClass::Color m_useColor;
	aetherClass::Color m_deadColor;


};

