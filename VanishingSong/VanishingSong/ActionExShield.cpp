#include "ActionExShield.h"


ActionExShield::ActionExShield():
ActionCommand(eCommandType::eSkill)
{
}


ActionExShield::~ActionExShield()
{
}

void ActionExShield::mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount){
	return;
}