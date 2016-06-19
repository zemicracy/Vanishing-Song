#pragma once

#include"GaugeBase.h"
#include"CharaStatus.h"
#include"RhythmManager.h"
class PlayerHPGauge : public GaugeBase
{
public:
	PlayerHPGauge();
	~PlayerHPGauge();

	bool mInitialize()override;
	void mRender(std::shared_ptr<HalfFillShader>)override;
	void mUpdate(float)override;

	//AccessorMethods
	void mSetRhythmManager(RhythmManager* rhythm);
	void mSetCharaStatus(CharaStatus*);
	void mSetProperty(aetherClass::SpriteBase::Property);

private:
	void mFinalize()override;
	void mException();
	void mRhythmicMotion();

	RhythmManager *m_rhythm;
	std::shared_ptr<aetherClass::SpriteBase>m_pDamageSprite;
	HalfFillShader::FillType m_fillType;
	CharaStatus *m_CharaStatus;
	aetherClass::SpriteBase::Property m_property;
	
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

