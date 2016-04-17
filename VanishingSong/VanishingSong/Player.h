#ifndef _PLAYER_H
#define _PLAYER_H

#include "GearFrame.h"
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"
#include "ActionCommand.h"
#include "Animation.h"
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
	/*	Player�̏��		*/
	enum class eState{
		eMove,
		eWait,
		eNull
	};

	/*	�Ƃ肠�����J�����p	*/
	struct Offset
	{
		aetherClass::Vector3 _translation;
		aetherClass::Vector3 _rotation;
	};

	struct Counter{
		Counter(){
			Reset();
		}

		~Counter(){
			Reset();
		}
		int _defaultFrame;
		int _commandFrame;
		bool _changeDefaultFrame;
		bool _changeCommandFrame;
	private:
		void Reset(){
			_defaultFrame = NULL;
			_commandFrame = NULL;
			_changeDefaultFrame = false;
			_changeCommandFrame = false;
		}
	};

	
	struct AnimationFrame{
		std::vector<Animation> _animation;
		int _animationFrame;
	};

	struct TransformState{
		Player::eState _state;
		aetherClass::Transform _transform;
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
	eCommandType mCommand(std::shared_ptr<ActionCommand>, const float timeScale);

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

	std::shared_ptr<aetherClass::Cube> mGetColldier();
private:
	/*
	�v���C���[�ɑ΂���L�[���͏���
	����ړ������Ɖ�]����
	*/
	TransformState mReadKey(const float timeScale);

	bool mInitializeGear(std::shared_ptr<GearFrame>&, aetherClass::ViewCamera*);

	bool mLoadModelProperty(std::shared_ptr<GearFrame>&, std::string modelDataFile);

	void mRotationAdjustment(std::shared_ptr<Gear>&);

	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);

	void mSetUpCollider(std::shared_ptr<aetherClass::Cube>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);

	void mRegisterAnimation(Player::eState key,const int allFrame, std::string first, std::string last);

	void mGetAnimationTransform(Player::eState state,aetherClass::Transform);

	void mRegisterParts(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&,Gear::eType, std::shared_ptr<Gear>&);

	void mLookAtView(aetherClass::ViewCamera&,aetherClass::Vector3 rotation,aetherClass::Vector3 lookAtPosition);
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
	std::shared_ptr<ActionCommand> m_pActionCommand;
	std::shared_ptr<Gear> m_pTopGear;
	aetherClass::ViewCamera m_playerView;

	aetherClass::Transform m_moveTransform;
	
	CharaStatus m_status;
	eCommandType m_prevCommand;
	eState m_prevState;
	CharaEntity m_charaEntity;
	Offset m_cameraOffset;
	aetherClass::Vector3 m_cameraRotation;
	Counter m_actionCount;			// �A�N�V�������s��������ۑ����Ƃ��p

	std::shared_ptr<aetherClass::Cube> m_pCubeCollider;

	std::unordered_map<eState, AnimationFrame> m_defaultAnimation;

	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_pGearPartsHash;
};

#endif