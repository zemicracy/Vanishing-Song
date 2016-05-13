#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <Singleton.h>
class GameManager
{
public:
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
public:
	GameManager();
	~GameManager();
	void mInitialize();
	void mGameState(eGameState);
	eGameState mGameState();
	bool mIsPause();	
private:
	eGameState m_gameState;
	bool m_isPause;
	
};

#endif