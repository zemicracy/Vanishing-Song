#include "ActionLeftStep.h"
#include <Transform.h>

using namespace aetherClass;
ActionLeftStep::ActionLeftStep():
ActionCommand(eCommandType::eLeftStep)
{
}


ActionLeftStep::~ActionLeftStep()
{
}

void ActionLeftStep::mAction(std::shared_ptr<GearFrame> player, float timeScale,int frame){

	Transform transform;
	for (auto index : GetAnimationValue())
	{
		
		transform = mGetCharaEntity().mGetTransformInterpolation(index._start, index._end, 45, frame);
		player->SetTransformValue(index._name, transform);
	}
	return;
}