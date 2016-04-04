#include "ActionShortAttack.h"


ActionShortAttack::ActionShortAttack() : ActionCommand(eActionType::eShortDistanceAttack)
{
}


ActionShortAttack::~ActionShortAttack()
{
}


void ActionShortAttack::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){
	
}

