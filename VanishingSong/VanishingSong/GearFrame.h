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
	std::shared_ptr<Gear> m_pRightArm;
	std::shared_ptr<Gear> m_pLeftArm;

	// ��
	std::shared_ptr<Gear> m_pRightFoot;
	std::shared_ptr<Gear> m_pLeftFoot;
	void Release();

	void SetTransformValue(Gear::eType, aetherClass::Transform);

};

