#include "ActionSkill.h"


ActionSkill::ActionSkill():
ActionCommand(eCommandType::eSkill)
{
}


ActionSkill::~ActionSkill()
{
}

void ActionSkill::mAction(std::shared_ptr<GearFrame>, float timeScale, int count){

}