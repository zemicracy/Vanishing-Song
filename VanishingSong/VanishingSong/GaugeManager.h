#pragma once

#include"HalfFillShader.h"
#include"PlayerHPGauge.h"
#include"CharaStatus.h"
#include<memory>
class GaugeManager
{
public:
	GaugeManager();
	~GaugeManager();

	bool mInitialize(RhythmManager *rhythm);
	void mRender();
	void mUpdate(float);

	//AccessorMethods
	void mSetHpAll(CharaStatus* player, CharaStatus* enemy);

private:
	void mFinalize();
	std::shared_ptr<HalfFillShader>m_shader;
	std::unique_ptr<PlayerHPGauge>m_playerHpGauge;
	std::unique_ptr<PlayerHPGauge>m_enemyHpGauge;
};

