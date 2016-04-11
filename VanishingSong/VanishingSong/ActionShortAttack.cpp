#include "ActionShortAttack.h"
#include "Vector3.h"
using namespace aetherClass;
ActionShortAttack::ActionShortAttack() : ActionCommand(eCommandType::eShortDistanceAttack)
{
}


ActionShortAttack::~ActionShortAttack()
{
}


void ActionShortAttack::mAction(std::shared_ptr<GearFrame> gearFrame, float timeScale, int count){

	Vector3 rotation;
	rotation = Interpolation<Vector3>(Vector3(0, 10, 0), Vector3(20, 10, 0), 60, count);
	mGetCharaEntity().mGearKeyframeTranslation(gearFrame->m_pLeftUpperArm, rotation);
}

