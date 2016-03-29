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
	 m_pWaist.reset();
	 m_pRightShoulder.reset();
	 m_pLeftShoulder.reset();
	 m_pRightUpperArm.reset();
	 m_pLeftUpperArm.reset();
	 m_pRightLowerArm.reset();
	 m_pLeftLowerArm.reset();
	 m_pRightUpperLeg.reset();
	 m_pLeftUpperLeg.reset();
	 m_pRightLowerLeg.reset();
	 m_pLeftLowerLeg.reset();

}
