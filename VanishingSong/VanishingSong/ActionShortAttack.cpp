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

	Transform transform;
	for (auto index : GetAnimationValue())
	{

		transform = mGetCharaEntity().mGetTransformInterpolation(index._start, index._end, 45, count);
		gearFrame->SetTransformValue(index._name, transform);
	}
}

