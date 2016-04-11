#include "ActionShield.h"


ActionShield::ActionShield():
ActionCommand(eCommandType::eShield)
{
}


ActionShield::~ActionShield()
{
}

void ActionShield::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){

}