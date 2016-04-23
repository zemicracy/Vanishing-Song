#pragma once

#include"HalfFillShader.h"
#include"PlayerHPGauge.h"
#include"PlayerMPGauge.h"
#include"CharaStatus.h"
#include<memory>
class GaugeManager
{
public:
	GaugeManager();
	~GaugeManager();

	bool mInitialize();
	void mRender();
	void mUpdate(float);

	//AccessorMethods
	void mSetCharaStatus(CharaStatus*);
	void mSetuseMp(float);
private:
	void mFinalize();
	std::shared_ptr<HalfFillShader>m_shader;
	std::unique_ptr<PlayerHPGauge>m_hpGauge;
	std::unique_ptr<PlayerMPGauge>m_mpGauge;
};

