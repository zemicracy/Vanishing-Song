#pragma once
#include "Gear.h"
#include<memory>
struct GearFrame
{
public:
	GearFrame();
	~GearFrame();

	// 体のパーツ
	std::shared_ptr<Gear> m_pBody;

	// 腰のパーツ
	std::shared_ptr<Gear> m_pWaist;

	// 腕のパーツ
	std::shared_ptr<Gear> m_pRightArm;
	std::shared_ptr<Gear> m_pLeftArm;

	// 足
	std::shared_ptr<Gear> m_pRightFoot;
	std::shared_ptr<Gear> m_pLeftFoot;
	void Release();

	void SetTransformValue(Gear::eType, aetherClass::Transform);

};

