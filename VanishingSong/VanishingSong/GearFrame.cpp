#include "GearFrame.h"


GearFrame::GearFrame()
{
}


GearFrame::~GearFrame()
{
	Release();
}

void GearFrame::Release()
{
	// �̃p�[�c�̉��
	if (m_pBody)
	{
		m_pBody->Release();
		m_pWaist.reset();
		m_pWaist = nullptr;
	}

	// ���p�[�c�̉��
	if (m_pWaist)
	{
		m_pWaist->Release();
		m_pWaist.reset();
		m_pWaist = nullptr;
	}

	// �E�r�i��j�̉��
	if (m_pRightArm)
	{
		m_pRightArm->Release();
		m_pRightArm.reset();
		m_pRightArm = nullptr;
	}

	// ���r�i��j�̉��
	if (m_pLeftArm)
	{
		m_pLeftArm->Release();
		m_pLeftArm.reset();
		m_pLeftArm = nullptr;
	}

	// ����̉��
	if (m_pLeftFoot)
	{
		m_pLeftFoot->Release();
		m_pLeftFoot.reset();
		m_pLeftFoot = nullptr;
	}

	// ����̉��
	if (m_pRightFoot)
	{
		m_pRightFoot->Release();
		m_pRightFoot.reset();
		m_pRightFoot = nullptr;
	}
}

void GearFrame::SetTransformValue(Gear::eType type, aetherClass::Transform value){
	switch (type)
	{
	case Gear::eType::eBody:
		m_pBody->_pGear->property._transform = value;
		break;
	case Gear::eType::eRightArm:
		m_pRightArm->_pGear->property._transform = value;
		break;
	case Gear::eType::eLeftArm:
		m_pLeftArm->_pGear->property._transform = value;
		break;
	
	case Gear::eType::eWaist:
		m_pWaist->_pGear->property._transform = value;
		break;
	case Gear::eType::eLeftFoot:
		m_pLeftFoot->_pGear->property._transform = value;
		break;
	case Gear::eType::eRightFoot:
		m_pRightFoot->_pGear->property._transform = value;
		break;

	case Gear::eType::eNull:
		break;
	default:
		break;
	}
}

