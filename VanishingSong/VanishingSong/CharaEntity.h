/*
	�M�A�Ɋւ���֐����܂Ƃ߂��N���X
*/
#ifndef _CHARAENTITY_H
#define _CHARAENTITY_H

#include <memory>
#include "GearFrame.h"
#include "Const.h"
#include "Animation.h"
#include <WorldReader.h>
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
		�ŏ��ɓn���ꂽ�p�[�c�𒆐S�Ɍ��]����
	*/
	void mGearRotation(std::shared_ptr<Gear> top,std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	/*
		�w��p�[�c�̂݉�]
		��O�����Ŏw�肵���p�[�c�Ƃ��̎q���͉�]�����Ȃ�
	*/
	void mGearPartsRotation(std::shared_ptr<Gear> top,std::shared_ptr<Gear> gear,Gear::eType notRotaionType, const aetherClass::Vector3 rotation);

	/*
		�A�z�z��ɓo�^�p
	*/
	void mRegisterParts(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, Gear::eType, std::shared_ptr<Gear>&);

	/*
		�l�̐ݒ�p
		�e�ƁA�ŏ�ʃp�[�c�̍������߁A�ۑ�����
	*/
	void mSetLoadGearValue(std::shared_ptr<Gear>& top, std::shared_ptr<Gear>& gear, ObjectInfo*);


	/*
	�A�j���[�V�����̒l�Z�b�g�p
	*/
	bool mLoadAnimation(std::vector<Animation>&animationVector,std::string startState, std::string endState);

	aetherClass::Transform mGetTransformInterpolation(aetherClass::Transform, aetherClass::Transform, const int allFrame, const int nowFrame);

private:
	Gear::eType mSetPartsValue(std::string, aetherClass::Transform* input, aetherClass::Transform value);
};


#endif