#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
#include "Const.h"
#include <unordered_map>
#include <Transform.h>
#include "FadeManager.h"
class GameManager
{
public:
	// ���݂̏��
	enum class eBattleState{
		eWin,
		eLose,
		eListen,    // �G�̉��t�𒮂����
		ePerform,   // �v���C���[�̉��t�J�n��� 
		eBattle,    // �퓬�̎��s���
		eCheck,     // ���̏�Ԃ𔻒肷��
		eResult,	// ���U���g
		eNewWave,
		eNull,
	};

	enum class eGameMode{
		eNull,
		eQuarter,  // �l��
		eEighter,   // ����
	};

	// �t�B�[���h�ł̃{�X�̏��
	enum class eBossState{
		eVisible,
		eUnVisible,
		eWin,
		eNull
	};

	enum class eFieldState{
		eTutorial,
		eTutorialEnd,

		eFirstStage,
		eSecoundStage,
		eThirdStage,
		eForthStage,
		
		eBoss,
		eNull
	};
public:

	static GameManager& mGetInstance();
	void mInitialize();	

	void mPushUsePlayer(eMusical);
	std::unordered_map<eMusical, eMusical>& mGetUsePlayer();
	aetherClass::Transform mGetPlayerTransform();
	void mSetPlayerTransform(aetherClass::Transform);

	void mBattleDataFile(std::string);
	std::string mBattleDataFile();

	void mBossState(eBossState);
	eBossState mBossState();

	void mFieldState(eFieldState);
	eFieldState mFieldState();

	void mNote(eMusical);
	std::vector<eMusical>& mNote();

	void mGetCanStage(const int);
	int  mGetCanStage()const;

	FadeManager& mfadeManager();

private:
	GameManager();
	~GameManager();
private:
	bool m_isPause;
	std::unordered_map<eMusical,eMusical> m_players;
	aetherClass::Transform m_prevPlayerTransform;
	std::string m_battleDataFile;
	eBossState m_bossState;
	eFieldState m_fieldState;
	std::vector<eMusical> m_noteArray;
	int m_canStageNumber;

	FadeManager m_fadeManager;
};

#endif