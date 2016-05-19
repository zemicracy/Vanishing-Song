#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
struct BattlePlayerDesc{
	//std::array<>
};
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
private:
	bool m_isPause;
	
};

#endif