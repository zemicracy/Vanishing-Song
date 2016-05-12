#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
class GameManager
{
public:
	// �I�����ꂽ���[�h�p
	enum class eGameMode{
		eBattle,
		eGame,
		ePractice,
		eTutorial,
		eNull
	};

	// ���݂̏��
	enum class eGameState{
		e1PWin,
		eClear,
		eGameOver,
		eListen,    // �G�̉��t�𒮂����
		ePerform,   // �v���C���[�̉��t�J�n��� 
		eBattle,    // �퓬�̎��s���
		eNull,
	};

	// �X�L���̃^�C�v
	enum class eSkillType{
		eExAttack,
		eExShield,
		eExHeel,
		eExBuff,
		eNull
	};

	// ����
	enum class eDay{
		eNull,
		e1day,
		e2day,
		e3day,
		e4day,
		e5day,
		e6day,
		e7day,
		eLastDay,
	};
public:
	GameManager();
	~GameManager();
	void mInitialize();
	signed char mGetToDay();
	void mGameState(eGameState);
	eGameState mGameState();
	bool mIsPause();
	void mNextDay();

	/*	�X�L���Z�b�g�A�擾�p*/
	eSkillType mSkillType()const;
	void mSkillType(const eSkillType);

	/*	�Q�[�����[�h�Z�b�g�A�擾�p*/
	eGameMode mGameMode()const;
	void mGameMode(const eGameMode);
	
private:
	eGameState m_gameState;
	bool m_isPause;
	signed char m_day;
	eSkillType m_skillType;
	eGameMode m_gameMode;
	
};

#endif