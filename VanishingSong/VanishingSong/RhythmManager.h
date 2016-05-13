#pragma once

#include<GameSound.h>
#include<memory>
class RhythmManager
{
public:
	RhythmManager();
	~RhythmManager();

	//AccesserMethod
	bool mIsQuarterBeat();	//�S��
	bool mIsEighterBeat();	//�W��
	bool mIsWholeBeat();	//�S����

	int mQuarterBeatTime();	//�S��
	int mEighterBeatTime();	//�W��
	int mWholeBeatTime();	//�S����


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

