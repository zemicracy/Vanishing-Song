#include "ActionExAttack.h"


ActionExAttack::ActionExAttack():
ActionCommand(eCommandType::eSkill)
{
}


ActionExAttack::~ActionExAttack()
{
}

void ActionExAttack::mAction(std::shared_ptr<GearFrame>, float timeScale, int frameCount){
	return;
}