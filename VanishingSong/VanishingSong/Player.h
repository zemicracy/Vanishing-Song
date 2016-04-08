#ifndef _PLAYER_H
#define _PLAYER_H

#include "GearFrame.h"
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"
#include "ActionCommand.h"
#include <Cube.h>
#include <ShaderBase.h>
#include <Transform.h>
#include <WorldReader.h>
#include <ViewCamera.h>
#include <unordered_map>
#include <vector>
class Player
{
private:
	enum class eState{
		eMove,
		eWait,
		eNull
	};
public:
	Player();
	~Player();
	/*
	������
	*/
	bool mInitialize();

	/*
	�X�V����
	*/
	void mUpdate(const float timeScale);

	/*
	�`�揈��
	*/
	void mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader);

	/*
	�A�N�V�������s�p
	*/
	eCommandType m_Command(std::shared_ptr<ActionCommand>, const float timeScale);

	/*
	���s�������̂̓o�^
	�������F�A�N�V�����̎��
	�������F���Ԗڂ̂��̂��̐���
	*/
	void mAddPrevActionCmmand(eCommandType, const int id);

	/*
	�A�N�V�������X�g�̏㏑��
	*/
	void mResetPrevActionList();

	/*
	�������
	*/
	void mFinalize();

	aetherClass::ViewCamera* mGetView();

	std::shared_ptr<aetherClass::ModelBase> mGetCollider(const int);

	int mGetColliderListSize()const;
private:
	/*
	�v���C���[�ɑ΂���L�[���͏���
	����ړ������Ɖ�]����
	*/
	aetherClass::Transform mReadKey(const float timeScale);

	bool mInitializeGear(std::shared_ptr<GearFrame>&, aetherClass::ViewCamera*);

	bool mLoadModelProperty(std::shared_ptr<GearFrame>&, std::string modelDataFile);

	void mRotationAdjustment(std::shared_ptr<Gear>&);

	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
	std::shared_ptr<ActionCommand> m_pActionCommand;
	std::shared_ptr<Gear> m_pTopGear;
	std::vector<std::shared_ptr<aetherClass::ModelBase>> m_playerCollideList;
	aetherClass::ViewCamera m_playerView;

	CharaStatus m_status;
	eCommandType m_prevCommand;
	CharaEntity m_charaEntity;
	eState m_state;

	int m_actionCount;// �A�N�V�������s��������ۑ����Ƃ��p
};

#endif