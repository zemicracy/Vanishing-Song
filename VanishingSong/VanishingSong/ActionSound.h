#pragma once

#include<GameSound.h>
#include"CharaStatus.h"
#include<unordered_map>

class ActionSound
{
public:
	ActionSound();
	~ActionSound();

	void mLoad(std::string path);
	bool mIsPlayEnd();
	void mPlaySoundAction(float);
	private:
		aetherClass::GameSound m_sound;
};

