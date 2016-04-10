#include "ActionLongAttack.h"


ActionLongAttack::ActionLongAttack():
ActionCommand(eCommandType::eLongDistanceAttack)
{
}


ActionLongAttack::~ActionLongAttack()
{
}


void ActionLongAttack::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){

}