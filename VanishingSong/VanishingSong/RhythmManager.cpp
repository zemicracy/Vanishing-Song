#include "RhythmManager.h"
#include"ResourceManager.h"
#include"Singleton.h"

RhythmManager::RhythmManager()
{
	m_playTime = 0;
	m_sound = nullptr;
	m_prevEighterBeat = 0;
	m_prevQuarterBeat= 0;
	m_prevWholeBeat = 0;
	m_prevSixteenthBeat = 0;
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
bool RhythmManager::mIsSixteenthBeat(){
	return m_IsSixteenthBeat;
}


float RhythmManager::mQuarterBeatTime(){
	return m_playTime;
}
float RhythmManager::mEighterBeatTime(){
	return m_playTime * 2;
}
float RhythmManager::mWholeBeatTime(){
	return m_playTime / 4;
}
float RhythmManager::mSixteenthBeatTime(){
	return m_playTime * 4;
}


float RhythmManager::mGetPlayTime(){
	return m_playTime;
}


void RhythmManager::mInitializeRhythm(int index, int bpm){
	m_sound = Singleton<ResourceManager>::GetInstance().mGetBGM(eMusical::eBlue);
	m_bpm = bpm;
	m_playTime = 0;
}

void RhythmManager::mAcquire(){
	DWORD currentTempWav, nextTempWav;
	m_sound->GetPlayPosition(&currentTempWav, &nextTempWav);
	m_playTime = (( float(currentTempWav) * 8 / 44100 / 2 / 16) / (60 / float(m_bpm)) + FLT_EPSILON);

	int QuarterbeatNo = int(m_playTime);
	int EighterbeatNo = int(m_playTime * 2);
	int SixteenthbeatNo = int(m_playTime * 4);
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

	if (m_prevSixteenthBeat != SixteenthbeatNo){
		m_prevSixteenthBeat = SixteenthbeatNo;
		m_IsSixteenthBeat = true;
	}
	else {
		m_IsSixteenthBeat = false;
	}


	if (m_prevWholeBeat != WholebeatNo){
		m_prevWholeBeat = WholebeatNo;
		m_IsWholeBeat = true;
	}
	else {
		m_IsWholeBeat = false;
	}

}
