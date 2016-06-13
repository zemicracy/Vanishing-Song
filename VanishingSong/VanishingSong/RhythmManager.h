#pragma once

#include<GameSound.h>
#include<memory>
class RhythmManager
{
public:
	RhythmManager();
	~RhythmManager();

	//AccesserMethod
	bool mIsWholeBeat();	//‘S‰¹•„
	bool mIsQuarterBeat();	//‚S•ª
	bool mIsEighterBeat();	//‚W•ª
	bool mIsSixteenthBeat();	//16•ª

	float mWholeBeatTime();	//‘S‰¹•„
	float mQuarterBeatTime();	//‚S•ª
	float mEighterBeatTime();	//‚W•ª
	float mSixteenthBeatTime();	//16•ª


	float mGetPlayTime();
	
	void mFinalize();
	void mInitializeRhythm(std::shared_ptr<aetherClass::GameSound> BGMindex, int bpm);
	void mAcquire();
private:
	std::shared_ptr<aetherClass::GameSound>m_sound;
	float m_playTime;
	
	int m_bpm;
	int m_prevSixteenthBeat;
	int m_prevQuarterBeat;
	int m_prevEighterBeat;
	int m_prevWholeBeat;

	bool m_IsSixteenthBeat;
	bool m_IsQuarterBeat;
	bool m_IsEighterBeat;
	bool m_IsWholeBeat;
};

