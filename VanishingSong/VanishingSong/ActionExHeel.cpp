#include "ActionExHeel.h"


ActionExHeel::ActionExHeel():
ActionCommand(eCommandType::eSkill)
{
}


ActionExHeel::~ActionExHeel()
{
}

void ActionExHeel::mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount){
	return;
}