#include "ActionLeftStep.h"


ActionLeftStep::ActionLeftStep():
ActionCommand(eCommandType::eLeftStep)
{
}


ActionLeftStep::~ActionLeftStep()
{
}

void ActionLeftStep::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){

}