#include "ActionShortAttack.h"


ActionShortAttack::ActionShortAttack() : ActionCommand(eCommandType::eShortDistanceAttack)
{
}


ActionShortAttack::~ActionShortAttack()
{
}


void ActionShortAttack::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){
	
}

