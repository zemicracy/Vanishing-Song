#pragma once
#include"ActionBoard.h"
#include"GameSound.h"
#include"GameManager.h"
#include"ActionSound.h"
#include"RhythmManager.h"

#include<array>
class OrderList
{
public:

	OrderList();
	~OrderList();
	//�Đ�
	void mPlay();

	//Listen
	void mAddEnemyOrder(std::vector<std::shared_ptr<ActionCommand>>&);	//�G���烊�X�g���󂯎��

	//Perform
	void mAddPlayerOrder(std::vector<std::shared_ptr<ActionCommand>>);
	std::shared_ptr<ActionCommand> mGetActionCommand();

	//Check

	//Battle

	//End
	void mEndReset();

	//Indispensable Method
	void mInitialize(GameManager::eGameMode,GameManager::eBattleState&,std::shared_ptr<ActionBoard>);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdate();
	//AccesserMethod

	bool mIsEnd();

private:
	void mBattleUpdate();
	void mListenUpdate();
	void mPerformUpdate();

	void mFinalize();
	
	//���Đ�
	void mPlaySound(std::shared_ptr<ActionSound>);
	//��~
	void mListStop();
	//���Y���ɍ��킹�����[�V�����͂�����
	void mRhythmicMotion();

private:

	//�G�ƃv���C���[�̃��X�g
	std::vector<std::shared_ptr<ActionCommand>>m_PlayerOrderList;
	std::vector<std::shared_ptr<ActionCommand>>m_EnemyOrderList;

	//�`���
	std::vector<std::shared_ptr<aetherClass::SpriteBase>>m_pSpriteList;
	std::vector<aetherClass::Vector3>m_pSpriteOrigin;

	//�ꎞ��ԏꏊ�i�O�ɏo���p�j
	std::shared_ptr<ActionCommand>m_playedAction;

	//���̂����[�V�����ړI
	std::shared_ptr<ActionBoard>m_ActionBoard;
	std::shared_ptr<aetherClass::SpriteBase>m_pVolumeImage;
	aetherClass::Vector3 m_VolumeOrigin;
	std::shared_ptr<aetherClass::SpriteBase>m_pBackImage;
	aetherClass::Vector3 m_BackImageOrigin;
	aetherClass::Vector3 m_BackImageScaleOrigin;

	//���Y��
	RhythmManager *m_rhythm;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTextureList;


	bool m_isAlPlay;		//���ɍĐ��ォ�H
	bool m_isStart;			//���蒆��
	bool m_isEnd;			//�I��������

	bool m_isKeyDown;		//���̔��ɂɃL�[�������ꂽ��
	bool m_isPlaySound;		//�R�}���h�Đ�d�ł��邩

	int m_processId;		//��������ID
	int m_MaxOrderSize;
	
	int m_enemyDamageCounter;
	int m_playerDamageCounter;



	//
	GameManager::eBattleState* m_faze;
	GameManager::eGameMode m_mode;
	//�萔

	const float m_kMissLevel = 0.5f;
	const float m_kGreat = 0.3f;

};

