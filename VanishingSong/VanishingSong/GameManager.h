#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
#include "Const.h"
#include <unordered_map>
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
		eNull,
	};

	enum class eGameMode{
		eNull,
		eQuarter,  // �l��
		eEighter,   // ����
	};
public:
	GameManager();
	~GameManager();
	void mInitialize();
	bool mIsPause();	

	void mPushUsePlayer(eMusical);
	std::unordered_map<eMusical, eMusical>& mGetUsePlayer();
private:
	bool m_isPause;
	std::unordered_map<eMusical,eMusical> m_players;
};

#endif