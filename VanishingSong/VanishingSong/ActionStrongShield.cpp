#include "ActionStrongShield.h"


ActionStrongShield::ActionStrongShield():
ActionCommand(eCommandType::eStrongShield)
{
}


ActionStrongShield::~ActionStrongShield()
{
}

void ActionStrongShield::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){
}