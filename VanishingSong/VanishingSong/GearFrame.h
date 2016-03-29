#pragma once
#include "Gear.h"
#include<memory>
class GearFrame
{
public:
	GearFrame();
	~GearFrame();

	void Release();


private:
	struct{
		std::shared_ptr<Gear> m_pWaist;
		std::shared_ptr<Gear> m_pRightShoulder;
		std::shared_ptr<Gear> m_pLeftShoulder;
		std::shared_ptr<Gear> m_pRightUpperArm;
		std::shared_ptr<Gear> m_pLeftUpperArm;
		std::shared_ptr<Gear> m_pRightLowerArm;
		std::shared_ptr<Gear> m_pLeftLowerArm;
		std::shared_ptr<Gear> m_pRightUpperLeg;
		std::shared_ptr<Gear> m_pLeftUpperLeg;
		std::shared_ptr<Gear> m_pRightLowerLeg;
		std::shared_ptr<Gear> m_pLeftLowerLeg;
	};

};

