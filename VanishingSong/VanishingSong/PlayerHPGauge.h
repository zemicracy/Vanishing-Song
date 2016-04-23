#pragma once

#include"GaugeBase.h"
#include"CharaStatus.h"
class PlayerHPGauge : public GaugeBase
{
public:
	PlayerHPGauge();
	~PlayerHPGauge();

	bool mInitialize()override;
	void mRender(std::shared_ptr<HalfFillShader>)override;
	void mUpdate(float)override;

	//AccessorMethods
	void mSetCharaStatus(CharaStatus*);

private:
	void mFinalize()override;
	void mException();

	std::shared_ptr<aetherClass::SpriteBase>m_pDamageSprite;
	HalfFillShader::FillType m_fillType;
	CharaStatus *m_CharaStatus;
	
	struct DamageValue{
		float _interpolation;
		float _intervalTime;
		float _prevHp,_morePrevHp;
		float _damageValue;
		bool _isPlay;
		DamageValue(){
			SecureZeroMemory(this, sizeof(DamageValue));
		}
	}m_damageValue;
};

