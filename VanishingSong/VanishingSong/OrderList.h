#pragma once
#include"ActionBoard.h"
#include"GameSound.h"
#include"GameManager.h"
#include"ActionSound.h"
#include"RhythmManager.h"
#include"AttackParticle.h"
#include"BattleField.h"
#include"ResultBoard.h"
#include"EffectGenerator2D.h"

#include<array>
namespace{
	enum eAppendOption{
		eNone = 0, eBlack = 1, eReverce = 2
	};
}
class OrderList
{
public:

	OrderList();
	~OrderList();
	//�Đ�
	void mPlay();
	void mLinePlay();

	typedef int eAppendoption;	//�t���O������Int
	//Listen
	void mSetOption(eAppendoption op);
	void mAddEnemyOrder(std::vector<std::shared_ptr<ActionCommand>>&);	//�G���烊�X�g���󂯎��

	//Perform
	std::shared_ptr<ActionCommand> mGetActionCommand();

	//Check
	//Battle
	int mGetDamage();

	//End
	void mEndReset();

	//Indispensable Method
	void mInitialize(GameManager::eGameMode,GameManager::eBattleState&,ActionBoard*,BattleField*,RhythmManager*);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUpdate();

	//AccesserMethod
	bool mIsEnd();
	ResultData mGetResult();

	//other
	void mRender3D(aetherClass::ShaderBase*);

private:
	void mBattleUpdate();
	void mListenUpdate();
	void mPerformUpdate();
	void mLineUpdate();

	void mFinalize();
	
	//���Đ�
	void mPlaySound(std::shared_ptr<ActionSound>);
	//��~
	void mListStop();
	//���Y���ɍ��킹�����[�V�����͂�����
	void mRhythmicMotion();
	void mAppendOptionInit();
private:

	//�G�ƃv���C���[�̃��X�g
	std::vector<std::shared_ptr<ActionCommand>>m_PlayerOrderList;
	std::vector<std::shared_ptr<ActionCommand>>m_EnemyOrderList;

	AttackParticle::ParticleDesc m_perticleDesc;
	std::unique_ptr<AttackParticle>m_pParticle;

	//�`���
	std::vector<std::shared_ptr<aetherClass::SpriteBase>>m_pSpriteList;
	std::vector<aetherClass::Vector3>m_pSpriteOrigin;

	//�ꎞ��ԏꏊ�i�O�ɏo���p�j
	std::shared_ptr<ActionCommand>m_playedAction;

	//���̂����[�V�����ړI
	ActionBoard* m_ActionBoard;
	BattleField* m_Field;
	std::shared_ptr<aetherClass::SpriteBase>m_pVolumeImage;
	aetherClass::Vector3 m_VolumeOrigin;

	std::shared_ptr<aetherClass::SpriteBase>m_pBackImage;
	aetherClass::Vector3 m_BackImageOrigin;
	aetherClass::Vector3 m_BackImageReverceOrigin;
	aetherClass::Vector3 m_BackImageScaleOrigin;

	std::shared_ptr<aetherClass::SpriteBase>m_pReadLine;
	aetherClass::Vector3 m_ReadLineOrigin;
	aetherClass::Vector3 m_ReadLineReverce;

	std::shared_ptr<aetherClass::Rectangle2D>m_pFlame;
	aetherClass::Vector3 m_flamePosOrigin;
	aetherClass::Vector3 m_flameScaleOrigin;


	//���Y��
	RhythmManager *m_rhythm;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTextureList;

	//option
	eAppendoption m_option;

	//�G�t�F�N�g
	std::shared_ptr<EffectGenerator2D> m_pEffect;
	aetherClass::Transform m_effectTrans;

	bool m_isLineStart;		//���C���Đ�
	int m_eighterCount;		//�J�E���g

	bool m_isAlPlay;		//���ɍĐ��ォ�H
	bool m_isStart;			//���蒆��
	bool m_isEnd;			//�I��������

	bool m_isKeyDown;		//���̔��ɂɃL�[�������ꂽ��
	bool m_isPlaySound;		//�R�}���h�Đ�d�ł��邩

	int m_processId;		//��������ID
	int m_MaxOrderSize;
	
	int m_damagedValue;		//�U�����_���[�W��
	ResultData m_resultData;

	//
	GameManager::eBattleState* m_faze;
	GameManager::eGameMode m_mode;
	//�萔

	const float m_kMissLevel = 0.5f;
	const float m_kGreat = 0.25f;

};

