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
	std::shared_ptr<Gear> m_pRightUpperArm;
	std::shared_ptr<Gear> m_pLeftUpperArm;
	std::shared_ptr<Gear> m_pRightLowerArm;
	std::shared_ptr<Gear> m_pLeftLowerArm;

	// 足のパーツ
	std::shared_ptr<Gear> m_pRightUpperLeg;
	std::shared_ptr<Gear> m_pLeftUpperLeg;
	std::shared_ptr<Gear> m_pRightLowerLeg;
	std::shared_ptr<Gear> m_pLeftLowerLeg;

	// 手のパーツ
	std::shared_ptr<Gear> m_pRightHand;
	std::shared_ptr<Gear> m_pLeftHand;

	void Release();

};

