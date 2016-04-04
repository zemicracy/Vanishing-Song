#ifndef _CHARAENTITY_H
#define _CHARAENTITY_H

#include <memory>
#include "GearFrame.h"
#include "Const.h"
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
	void mGearMove(std::shared_ptr<Gear> gear, aetherClass::Vector3 move);

};


#endif