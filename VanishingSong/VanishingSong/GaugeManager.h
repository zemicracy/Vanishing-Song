#pragma once

#include"HalfFillShader.h"
#include"HPGauge.h"
#include<memory>
class GaugeManager
{
public:
	GaugeManager();
	~GaugeManager();

	bool mInitialize();
	void mRender();
	void mUpdate(float);
private:
	void mFinalize();
	std::shared_ptr<HalfFillShader>m_shader;
	std::unique_ptr<HPGauge>m_hpGauge;
};

