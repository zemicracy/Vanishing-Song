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
	// 体パーツの解放
	if (m_pBody)
	{
		m_pBody->Release();
		m_pWaist.reset();
		m_pWaist = nullptr;
	}

	// 腰パーツの解放
	if (m_pWaist)
	{
		m_pWaist->Release();
		m_pWaist.reset();
		m_pWaist = nullptr;
	}

	// 右腕（上）の解放
	if (m_pRightArm)
	{
		m_pRightArm->Release();
		m_pRightArm.reset();
		m_pRightArm = nullptr;
	}

	// 左腕（上）の解放
	if (m_pLeftArm)
	{
		m_pLeftArm->Release();
		m_pLeftArm.reset();
		m_pLeftArm = nullptr;
	}

	// 左手の解放
	if (m_pLeftFoot)
	{
		m_pLeftFoot->Release();
		m_pLeftFoot.reset();
		m_pLeftFoot = nullptr;
	}

	// 左手の解放
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

