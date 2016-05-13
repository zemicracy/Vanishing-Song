#pragma once

#include<GameSound.h>
#include<memory>
class RhythmManager
{
public:
	RhythmManager();
	~RhythmManager();

	//AccesserMethod
	bool mIsQuarterBeat();	//‚S•ª
	bool mIsEighterBeat();	//‚W•ª
	bool mIsWholeBeat();	//‘S‰¹•„

	int mQuarterBeatTime();	//‚S•ª
	int mEighterBeatTime();	//‚W•ª
	int mWholeBeatTime();	//‘S‰¹•„


	float mGetPlayTime();
	
	
	void mInitializeRhythm(std::shared_ptr<aetherClass::GameSound>,int bpm);
	void mAcquire();
private:
	std::shared_ptr<aetherClass::GameSound>m_sound;
	float m_playTime;
	
	int m_bpm;
	int m_prevQuarterBeat;
	int m_prevEighterBeat;
	int m_prevWholeBeat;

	bool m_IsQuarterBeat;
	bool m_IsEighterBeat;
	bool m_IsWholeBeat;
};

