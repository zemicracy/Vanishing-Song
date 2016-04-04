#pragma once

#include<GameSound.h>
#include"CharaStatus.h"
#include<unordered_map>

class ActionSound
{
public:
	ActionSound();
	~ActionSound();

	void PlaySoundAction(eActionType,float);
	private:
		aetherClass::GameSound m_sound;
};

