#include "RhythmManager.h"


RhythmManager::RhythmManager()
{
	m_playTime = 0;
	m_sound = nullptr;
	m_prevEighterBeat = 0;
	m_prevQuarterBeat= 0;
	m_prevWholeBeat = 0;
}


RhythmManager::~RhythmManager()
{
}


bool RhythmManager::mIsQuarterBeat(){
	return m_IsQuarterBeat;
}
bool RhythmManager::mIsEighterBeat(){
	return m_IsEighterBeat;
}
bool RhythmManager::mIsWholeBeat(){
	return m_IsWholeBeat;
}

int RhythmManager::mQuarterBeatTime(){
	return int(m_playTime);
}
int RhythmManager::mEighterBeatTime(){
	return int(m_playTime * 2);
}
int RhythmManager::mWholeBeatTime(){
	return int(m_playTime / 4);
}


float RhythmManager::mGetPlayTime(){
	return m_playTime;
}


void RhythmManager::mInitializeRhythm(std::shared_ptr<aetherClass::GameSound> sound, int bpm){
	m_sound = sound;
	m_bpm = bpm;
	m_playTime = 0;
}

void RhythmManager::mAcquire(){
	DWORD currentTempWav, nextTempWav;
	m_sound->GetPlayPosition(&currentTempWav, &nextTempWav);
	m_playTime = ((((float)currentTempWav * 8 / 44100 / 2 / 16) / (60 / m_bpm) + FLT_EPSILON));

	int QuarterbeatNo = int(m_playTime);
	int EighterbeatNo = int(m_playTime * 2);
	int WholebeatNo = int(m_playTime / 4);

	if (m_prevQuarterBeat != QuarterbeatNo){
		m_prevQuarterBeat = QuarterbeatNo;
		m_IsQuarterBeat = true;
	}
	else{
		m_IsQuarterBeat = false;
	}

	if (m_prevEighterBeat != EighterbeatNo){
		m_prevEighterBeat = EighterbeatNo;
		m_IsEighterBeat = true;
	}
	else {
		m_IsEighterBeat = false;
	}

	if (m_prevWholeBeat != WholebeatNo){
		m_prevWholeBeat = WholebeatNo;
		m_IsWholeBeat = true;
	}
	else {
		m_IsWholeBeat = false;
	}

}
