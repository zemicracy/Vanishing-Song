#include "ActionSound.h"


ActionSound::ActionSound()
{
}


ActionSound::~ActionSound()
{
}

void ActionSound::PlaySoundAction(eActionType type,float volume){
//	m_sound.Load()
	m_sound.SetValume(volume);
	m_sound.PlayToOneTime();
}
