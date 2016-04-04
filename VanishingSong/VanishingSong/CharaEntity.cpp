#include "CharaEntity.h"
#include "Debug.h"
#include "Const.h"
using namespace aetherClass;

CharaEntity::CharaEntity()
{
}


CharaEntity::~CharaEntity()
{
}

std::shared_ptr<Gear> CharaEntity::mSetUpGear(std::string path, Gear::eType gearType, aetherClass::ViewCamera* view){
	std::shared_ptr<Gear> pGear;
	bool success;

	// �����̃p�[�c�̏�����
	pGear = std::make_shared<Gear>();
	pGear->_pGear = std::make_shared<FbxModel>();
	// ���g�̎�ނ̐ݒ�
	pGear->_type = gearType;

	// ���f���̓ǂݎ��
	success = pGear->_pGear->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!success)
	{
		Debug::mErrorPrint("FBX�̏�����������ɍs���܂���ł����B", __FILE__, __FUNCTION__, __LINE__);

		if (kCharaDebug)
		{
			// �R���C�_�[�̏�����
			pGear->_pColider = std::make_shared<Cube>();
			pGear->_pColider->Initialize();
			// �R���C�_�[�̐F��Ԃɐݒ�
			pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
			pGear->_pColider->SetCamera(view);
			return pGear;
		}
		return nullptr;
	}

	pGear->_pGear->SetCamera(view);
	// �e�N�X�`���̓ǂݍ���
	//pGear->_pGear->SetTextureDirectoryName("texture")

	// �R���C�_�[�̏�����
	pGear->_pColider = std::make_shared<Cube>();
	pGear->_pColider->Initialize();
	// �R���C�_�[�̐F��Ԃɐݒ�
	pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
	pGear->_pColider->SetCamera(view);
	return pGear;
}

/*
	�e�q�֌W�\�z�p
*/
void CharaEntity::mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!parentGear || !child||
		!parentGear->_pGear,!child->_pGear)return;


	parentGear->_pChildren.push_back(child);
	/*	�p�[�c��A�����鏈��	*/
	child->_pParent = parentGear;


	// ���P����ׂ��ӏ�
	// �ǂ�����ׂ���
	/*Vector3 gearPosition = parentGear->_pGear->property._transform._translation;
	Vector3 coliderPosition = parentGear->_pColider->property._transform._translation;

	child->_pGear->property._transform._position = gearPosition;
	child->_pColider->property._transform._position = coliderPosition;*/

	return;
}

/*
	�M�A�̕`��p
	�����ł���gear��`�悵�A���̂��Ƃ���gear�Ɏq��������΂��̕`�������
*/
void CharaEntity::mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){

	// �f�o�b�O���[�h�̎��̓R���C�_�[�̕\��
	if (kCharaDebug)
	{
		gear->_pColider->Render(colider_shader);
	}

	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear)return;

	gear->_pGear->Render(colider_shader);

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mGearRender(child, model_shader, colider_shader);
	}

	return;
}

/*
	�M�A�̈ړ��p�֐�
	�d�g�݂�mGearRender�ƈꏏ
*/
void CharaEntity::mGearMove(std::shared_ptr<Gear> gear, const Vector3 move){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear)return;

	// �M�A�Ƃ��̃R���C�_�[�𓮂���
	gear->_pGear->property._transform._translation += move;
	gear->_pColider->property._transform._translation += move;

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mGearMove(child, move);
	}

	return;
}

/*
	�M�A�����I�u�W�F�N�g�̉�]�p	
	�d�g�݂�mGearRender�ƈꏏ
*/
void CharaEntity::mGearRotation(std::shared_ptr<Gear> gear, const Vector3 rotation){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear)return;

	/*if (gear->_pParent)
	{
		gear->_pGear->property._transform._position = gear->_pParent->_pGear->property._transform._translation;
		gear->_pColider->property._transform._position = gear->_pParent->_pColider->property._transform._translation;
	}*/

	// �M�A�Ƃ��̃R���C�_�[�𓮂���
	gear->_pGear->property._transform._rotation += rotation;
	gear->_pColider->property._transform._rotation += rotation;

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mGearRotation(child, rotation);
	}

	return;
}