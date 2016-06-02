#ifndef _FIELDPLAYER_H
#define _FIELDPLAYER_H

#include <Cube.h>
#include <Sphere.h>
#include <ShaderBase.h>
#include <Transform.h>
#include <ViewCamera.h>
#include <unordered_map>
#include <vector>
#include "GearFrame.h"
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"
#include "Animation.h"

class FieldPlayer
{
private:

	/*	FieldPlayer�̏��		*/
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
		bool _changeDefaultFrame;
	private:
		void Reset(){
			_defaultFrame = NULL;
			_changeDefaultFrame = false;
		}
	};
	
	struct KeyValues{

		aetherClass::Transform _transform;
		aetherClass::Vector3 _cameraRotation;
	};

public:
	FieldPlayer() = default;
	~FieldPlayer();
	/*
	������
	*/
	bool mInitialize(std::shared_ptr<GearFrame> gearFrame,aetherClass::Vector3 position);

	/*
	�X�V����
	*/
	void mUpdate(const float timeScale, const bool isWait);

	/*
	�`�揈��
	*/
	void mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader);
	/*
		�J�����I�u�W�F�N�g�̃A�h���X�擾�p
	*/
	aetherClass::ViewCamera* mGetView();

	/*
		�R���C�_�[�̎擾�p
	*/
	std::shared_ptr<aetherClass::Cube> mGetBodyColldier();
	std::shared_ptr<aetherClass::Sphere> mGetSphereColldier();
	// �ǂɓ����������̏���
	void mOnHitWall();
	
	void mSetFieldNumber(const int);
	int mGetFieldNumber()const; 
	void mSetTransform(aetherClass::Transform);
	aetherClass::Transform mGetTransform();
private:


	/*
	�������
	*/
	void mFinalize();

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
	void mSetUpBodyCollider(std::shared_ptr<aetherClass::Sphere>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);

	/*
		�R���C�_�[�̍X�V����
	*/
	void mUpdateBodyCollider(aetherClass::Transform&);
	/*
		�A�j���[�V�����̓o�^
	*/
	void mRegisterAnimation(FieldPlayer::eState key,const int allFrame, std::string first, std::string last);

	/*
		�A�j���[�V�����Đ��p
	*/
	void mDefaultAnimation(FieldPlayer::eState& state);

	/*
		�J�����I�u�W�F�N�g�̍X�V
	*/
	void mUpdateView(aetherClass::ViewCamera&,aetherClass::Vector3& rotation,aetherClass::Vector3 lookAtPosition);

	/*
	�L�[��}�E�X�̏����̓ǂݎ��
	*/
	KeyValues mReadKey(const float timeScale);
	
	void mCheckCameraRotation(aetherClass::Vector3&);
private:
	std::unordered_map<Gear::eType, std::shared_ptr<Gear>> m_pGearHash;   // ���ꂼ��̃M�A�̃|�C���^�������₷���悤�ɂ܂Ƃ߂��A�z�z��

	std::shared_ptr<Gear> m_topGear;            // �ŏ�ʃp�[�c�̃|�C���^�����Ă���

	bool m_isHitWall;
	aetherClass::ViewCamera m_playerView;		//�@�J�����I�u�W�F�N�g

	aetherClass::Transform m_playerTransform;   // �v���C���[�̉�]�A�ړ��A�X�P�[�����Ǘ�
	aetherClass::Vector3 m_prevPosition;     // �O��̃g�����X�t�H�[�����
	aetherClass::Transform m_initialTransform; 

	aetherClass::Vector3 m_cameraRotation;		//�@�J�����̉�]���Ǘ�

	eState m_prevState;							// �O��̃v���C���[�̏��
	CharaEntity m_charaEntity;					// �֗��֐��̂���N���X�I�u�W�F�N�g
	Offset m_cameraOffset;						//�@�J�����̃I�t�Z�b�g
	Counter m_actionCount;			// ���ꂼ��̃A�N�V�������s�����t���[������ۑ����Ƃ��p
	std::shared_ptr<aetherClass::Cube> m_pBodyCollider;   // ��{�I�ȃR���C�_�[
	std::shared_ptr<aetherClass::Sphere> m_pSphereCollider;   // ��{�I�ȃR���C�_�[
	std::unordered_map<eState, AnimationFrame> m_defaultAnimation;   // ��{�I�ȃA�j���[�V�����̒l���܂񂾘A�z�z��
	eActionType m_action;

	int m_fieldNumber;
	
};

#endif