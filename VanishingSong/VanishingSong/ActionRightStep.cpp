#include "ActionRightStep.h"


ActionRightStep::ActionRightStep():
ActionCommand(eCommandType::eRightStep)
{
}


ActionRightStep::~ActionRightStep()
{
}

void ActionRightStep::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){

}