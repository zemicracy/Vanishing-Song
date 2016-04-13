#ifndef _CHARAENTITY_H
#define _CHARAENTITY_H

#include <memory>
#include "GearFrame.h"
#include "Const.h"
#include "Animation.h"
class CharaEntity
{
public:
	CharaEntity();
	~CharaEntity();

	/*
		�M�A�̏������p
	*/
	 std::shared_ptr<Gear> mSetUpGear(std::string path, Gear::eType, aetherClass::ViewCamera*);

	 /*
		�e�q�֌W�\�z�p
		�����͂����|�C���^�̑�������Ă��邾��
	 */
	void mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child);

	/*
		�M�A�����I�u�W�F�N�g�̃M�A�̕`��p
		�f�o�b�O���[�h���̓R���C�_�[���`��
		�d�g�݂͈����̃M�A�Ɏq��������΍ċA���[�v������
	
	*/
	void mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase*, aetherClass::ShaderBase*);

	/*
		�M�A�����I�u�W�F�N�g�̈ړ��p
		�d�g�݂�mGearRender�ƈꏏ
	*/
	void mGearMove(std::shared_ptr<Gear> gear, const aetherClass::Vector3 move);

	/*
	�M�A�����I�u�W�F�N�g�̈ړ��p
	�d�g�݂�mGearRender�ƈꏏ
	*/
	void mGearKeyframeTranslation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 move);


	/*
		�M�A�����I�u�W�F�N�g�̉�]�p
		�̑S�̂��ψ�ɉ�]����
		�d�g�݂�mGearRender�ƈꏏ
	*/
	void mBodyGearRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	/*
		�M�A�����I�u�W�F�N�g�̉�]�p
		������������]�ł���
		�d�g�݂�mGearRender�ƈꏏ
	*/
	void mPartsGearRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	/*
	�M�A�����I�u�W�F�N�g�̉�]�p
	�̑S�̂��ψ�ɉ�]����
	�d�g�݂�mGearRender�ƈꏏ
	�L�[�t���[���A�j���[�V�����p
	*/
	void mBodyGearKeyframeRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	/*
	�M�A�����I�u�W�F�N�g�̉�]�p
	������������]�ł���
	�d�g�݂�mGearRender�ƈꏏ
	�L�[�t���[���A�j���[�V�����p
	*/
	void mPartsGearKeyframeRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	/*
	�A�j���[�V�����̒l�Z�b�g�p
	*/
	bool LoadAnimation(std::vector<Animation>&animationVector,std::string startState, std::string endState);

	Gear::eType SetPartsValue(std::string, aetherClass::Transform* input, aetherClass::Transform value);

	aetherClass::Transform mGetTransformInterpolation(aetherClass::Transform, aetherClass::Transform, const int allFrame, const int nowFrame);
};


#endif