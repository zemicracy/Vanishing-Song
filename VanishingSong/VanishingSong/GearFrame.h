#pragma once
#include "Gear.h"
#include<memory>
struct GearFrame
{
public:
	GearFrame();
	~GearFrame();

	// �̂̃p�[�c
	std::shared_ptr<Gear> m_pBody;

	// ���̃p�[�c
	std::shared_ptr<Gear> m_pWaist;

	// �r�̃p�[�c
	std::shared_ptr<Gear> m_pRightUpperArm;
	std::shared_ptr<Gear> m_pLeftUpperArm;
	std::shared_ptr<Gear> m_pRightLowerArm;
	std::shared_ptr<Gear> m_pLeftLowerArm;

	// ���̃p�[�c
	std::shared_ptr<Gear> m_pRightUpperLeg;
	std::shared_ptr<Gear> m_pLeftUpperLeg;
	std::shared_ptr<Gear> m_pRightLowerLeg;
	std::shared_ptr<Gear> m_pLeftLowerLeg;

	// ��̃p�[�c
	std::shared_ptr<Gear> m_pRightHand;
	std::shared_ptr<Gear> m_pLeftHand;

	void Release();

};

