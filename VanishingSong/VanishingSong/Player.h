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
#include <ViewCamera.h>
#include <unordered_map>
#include <vector>
#include "Equipment.h"
namespace{
	const int kMaxBullet = 10;
}
class Player
{
private:
	struct Weapons{
		std::shared_ptr<Equipment> _sord;
		std::shared_ptr<Equipment> _gun;
		std::shared_ptr<Equipment> _shield;
	};
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
	
	struct KeyValues{
		Player::eState _state;
		aetherClass::Transform _transform;
		aetherClass::Vector3 _cameraRotation;
	};


public:
	// 
	struct BulletPool{
		std::shared_ptr<Equipment> _bullet;
		bool _isRun;
		int _number;
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
	void mUpdate(const float timeScale,std::shared_ptr<ActionCommand> command);

	/*
	�`�揈��
	*/
	void mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader);

	/*
	�A�N�V�������s�p
	*/
	eCommandType mCommand(std::shared_ptr<ActionCommand>, const float timeScale);

	eCommandType mGetNowCommandType();
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

	/*
		�J�����I�u�W�F�N�g�̃A�h���X�擾�p
	*/
	aetherClass::ViewCamera* mGetView();

	/*
		�R���C�_�[�̎擾�p
	*/
	std::shared_ptr<aetherClass::Cube> mGetBodyColldier();

	// �ǂɓ����������̏���
	void OnHitWall();

	std::array<BulletPool, kMaxBullet>& mGetBullet();
private:
	
	/*
		�J�����I�u�W�F�N�g�̏�����
	*/
	void mInitialPlayerView(CameraValue);

	/*
		�S�ẴM�A�̏�����
	*/
	bool mInitializeGearFrame(std::shared_ptr<GearFrame>&, aetherClass::ViewCamera*);

	/*
		�G�f�B�^�[����̒l��ǂݎ��悤
	*/
	bool mLoadProperty(std::shared_ptr<GearFrame>&, std::string modelDataFile);
	
	/*
		�R���C�_�[�̏�����
	*/
	void mSetUpBodyCollider(std::shared_ptr<aetherClass::Cube>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);

	/*
		�R���C�_�[�̍X�V����
	*/
	void mUpdateBodyCollider(aetherClass::Transform&);
	/*
		�A�j���[�V�����̓o�^
	*/
	void mRegisterAnimation(Player::eState key,const int allFrame, std::string first, std::string last);

	/*
		�A�j���[�V�����Đ��p
	*/
	void mDefaultAnimation(Player::eState& state);

	/*
		�J�����I�u�W�F�N�g�̍X�V
	*/
	void mUpdateView(aetherClass::ViewCamera&,aetherClass::Vector3& rotation,aetherClass::Vector3 lookAtPosition);

	/*
	�L�[��}�E�X�̏����̓ǂݎ��
	*/
	KeyValues mReadKey(const float timeScale);
	
	void mCheckCameraRotation(aetherClass::Vector3&);

	void mUpdateBullet(const float ,aetherClass::Matrix4x4&,std::array<BulletPool, kMaxBullet>&);

	// �e�ȊO�̏������p
	template<class type>
	void mSetupWeapon(std::shared_ptr<Equipment>& weapon, std::string model);
	void mSetupBullet(aetherClass::ViewCamera*);
	void mWeaponRun(eCommandType,const int callFrame);
private:
	std::shared_ptr<GearFrame> m_pGearFrame;   // �p�[�c�̊Ǘ�
	std::shared_ptr<ActionCommand> m_pActionCommand;  // �R�}���h���s�p
	std::shared_ptr<Gear> m_pTopGear;            // �ŏ�ʃp�[�c�̃|�C���^�����Ă���
	std::shared_ptr<Equipment> m_equipment;     // ���݂̑������̕���

	bool m_isHitWall;
	bool m_isCall;
	std::array<BulletPool,kMaxBullet> m_pBullets;
	aetherClass::ViewCamera m_playerView;		//�@�J�����I�u�W�F�N�g

	aetherClass::Transform m_playerTransform;   // �v���C���[�̉�]�A�ړ��A�X�P�[�����Ǘ�
	aetherClass::Transform m_prevTransform;     // �O��̃g�����X�t�H�[�����
	aetherClass::Vector3 m_cameraRotation;		//�@�J�����̉�]���Ǘ�

	CharaStatus m_status;                      // �v���C���[�̃X�e�[�^�X
	eCommandType m_prevCommand;					// �O����s�����R�}���h�̎��
	eState m_prevState;							// �O��̃v���C���[�̏��
	CharaEntity m_charaEntity;					// �֗��֐��̂���N���X�I�u�W�F�N�g
	Offset m_cameraOffset;						//�@�J�����̃I�t�Z�b�g
	Counter m_actionCount;			// ���ꂼ��̃A�N�V�������s�����t���[������ۑ����Ƃ��p
	aetherClass::DirectXEntity m_directXEntity;
	std::shared_ptr<aetherClass::Cube> m_pBodyCollider;   // ��{�I�ȃR���C�_�[

	std::unordered_map<eState, AnimationFrame> m_defaultAnimation;   // ��{�I�ȃA�j���[�V�����̒l���܂񂾘A�z�z��

	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_pGearHash;   // ���ꂼ��̃M�A�̃|�C���^�������₷���悤�ɂ܂Ƃ߂��A�z�z��
};

#endif