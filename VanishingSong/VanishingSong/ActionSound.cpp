#include "ActionSound.h"
#include"Debug.h"

ActionSound::ActionSound()
{
}


ActionSound::~ActionSound()
{
}
void ActionSound::mLoad(std::string path){
	bool result = m_sound.Load(path.c_str());
	if (!result) Debug::mErrorPrint("Could not load SoundFiles ,Please check FilePath or Name",path.c_str());
}
void ActionSound::mPlaySoundAction(float volume){
	m_sound.SetValume(volume);
	m_sound.PlayToOneTime();
}

bool ActionSound::mIsPlayEnd(){
	return m_sound.IsPlayEnd();
}
