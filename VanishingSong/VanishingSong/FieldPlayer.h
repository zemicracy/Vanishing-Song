#ifndef _FIELDPLAYER_H
#define _FIELDPLAYER_H

#include <Cube.h>
#include <Sphere.h>
#include <ShaderBase.h>
#include <Transform.h>
#include <ViewCamera.h>
#include <unordered_map>
#include <vector>
#include "CharaStatus.h"
#include "CharaEntity.h"
#include "Const.h"

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

public:
	FieldPlayer() = default;
	~FieldPlayer();
	/*
	������
	*/
	bool mInitialize(std::shared_ptr<aetherClass::FbxModel> model,aetherClass::Transform trans);

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
	// �ǂɓ����������̏���
	void mOnHitWall();
	
	void mSetFieldNumber(const int);
	int mGetFieldNumber()const; 
	aetherClass::Transform mGetTransform();
private:
	/*
	�������
	*/
	void mFinalize();

	/*
		�J�����I�u�W�F�N�g�̏�����
	*/
	void mInitialPlayerView(CameraValue,aetherClass::Vector3);

	/*
		�R���C�_�[�̏�����
	*/

	void mSetUpBodyCollider(std::shared_ptr<aetherClass::Cube>& collider, aetherClass::Vector3 original, aetherClass::Vector3 offset);
	
	/*
		�R���C�_�[�̍X�V����
	*/
	void mUpdateBodyCollider(aetherClass::Transform&);
	/*
		�J�����I�u�W�F�N�g�̍X�V
	*/
	void mUpdateView(aetherClass::ViewCamera&,aetherClass::Vector3& rotation,aetherClass::Vector3 lookAtPosition);

	/*
	�L�[��}�E�X�̏����̓ǂݎ��
	*/
	std::pair<aetherClass::Transform,aetherClass::Vector3> mReadKey(const float timeScale);

	void mCheckCameraRotation(aetherClass::Vector3&);
private:
	
	std::shared_ptr<aetherClass::FbxModel> m_model;
	bool m_isHitWall;
	aetherClass::ViewCamera m_playerView;		//�@�J�����I�u�W�F�N�g
	aetherClass::Vector3 m_prevPosition;     // �O��̃g�����X�t�H�[�����
	aetherClass::Vector3 m_cameraRotation;		//�@�J�����̉�]���Ǘ�
	CharaEntity m_charaEntity;					// �֗��֐��̂���N���X�I�u�W�F�N�g
	Offset m_cameraOffset;						//�@�J�����̃I�t�Z�b�g
	std::shared_ptr<aetherClass::Cube> m_pBodyCollider;   // ��{�I�ȃR���C�_�[
	float m_prevRotationY;
	int m_fieldNumber;
};

#endif