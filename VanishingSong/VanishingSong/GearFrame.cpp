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
	if (m_pRightUpperArm)
	{
		m_pRightUpperArm->Release();
		m_pRightUpperArm.reset();
		m_pRightUpperArm = nullptr;
	}

	// ���r�i��j�̉��
	if (m_pLeftUpperArm)
	{
		m_pLeftUpperArm->Release();
		m_pLeftUpperArm.reset();
		m_pLeftUpperArm = nullptr;
	}

	// �E�r�i���j�̉��
	if (m_pRightLowerArm)
	{
		m_pRightLowerArm->Release();
		m_pRightLowerArm.reset();
		m_pRightLowerArm = nullptr;
	}

	// ���r�i���j�̉��
	if (m_pLeftLowerArm)
	{
		m_pLeftLowerArm->Release();
		m_pLeftLowerArm.reset();
		m_pLeftLowerArm = nullptr;
	}

	// �E���i��j�̉��
	if (m_pRightUpperLeg)
	{
		m_pRightUpperLeg->Release();
		m_pRightUpperLeg.reset();
		m_pRightUpperLeg = nullptr;
	}

	// �����i��j�̉��
	if (m_pLeftUpperLeg)
	{
		m_pLeftUpperLeg->Release();
		m_pLeftUpperLeg.reset();
		m_pLeftUpperLeg = nullptr;
	}


	// �E���i���j�̉��
	if (m_pRightLowerLeg)
	{
		m_pRightLowerLeg->Release();
		m_pRightLowerLeg.reset();
		m_pRightLowerLeg = nullptr;
	}

	// �����i���j�̉��
	if (m_pLeftLowerLeg)
	{
		m_pLeftLowerLeg->Release();
		m_pLeftLowerLeg.reset();
		m_pLeftLowerLeg = nullptr;
	}

	// �E��̉��
	if (m_pRightHand)
	{
		m_pRightHand->Release();
		m_pRightHand.reset();
		m_pRightHand = nullptr;
	}

	// ����̉��
	if (m_pLeftHand)
	{
		m_pLeftHand->Release();
		m_pLeftHand.reset();
		m_pLeftHand = nullptr;
	}
}

void GearFrame::SetTransformValue(Gear::eType type, aetherClass::Transform value){
	switch (type)
	{
	case Gear::eType::eBody:
		m_pBody->_pGear->property._transform = value;
		break;
	case Gear::eType::eRightUpperArm:
		m_pRightUpperArm->_pGear->property._transform = value;
		break;
	case Gear::eType::eLeftUpperArm:
		m_pLeftUpperArm->_pGear->property._transform = value;
		break;
	case Gear::eType::eRightLowerArm:
		m_pRightLowerArm->_pGear->property._transform = value;
		break;
	case Gear::eType::eLeftLowerArm:
		m_pLeftLowerArm->_pGear->property._transform = value;

		break;
	case Gear::eType::eWaist:
		m_pWaist->_pGear->property._transform = value;
		break;
	case Gear::eType::eRightUpperLeg:
		m_pRightUpperLeg->_pGear->property._transform = value;
		break;
	case Gear::eType::eLeftUpperLeg:
		m_pLeftUpperLeg->_pGear->property._transform = value;
		break;
	case Gear::eType::eRightLowerLeg:
		m_pRightLowerLeg->_pGear->property._transform = value;
		break;
	case Gear::eType::eLeftLowerLeg:
		m_pLeftLowerLeg->_pGear->property._transform = value;
		break;
	case Gear::eType::eRightHand:
		m_pRightHand->_pGear->property._transform = value;
		break;
	case Gear::eType::eLeftHand:
		m_pLeftHand->_pGear->property._transform = value;
		break;
	case Gear::eType::eNull:
		break;
	default:
		break;
	}
}
