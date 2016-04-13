#include "CharaEntity.h"
#include "Debug.h"
#include "Const.h"
#include "Utility.h"
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

	// �p�[�c�̖{��
	pGear->_pGear = std::make_shared<FbxModel>();

	// ���g�̎�ނ̐ݒ�
	pGear->_type = gearType;

	// ���f���̓ǂݎ��
	success = pGear->_pGear->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!success)
	{
		Debug::mErrorPrint("FBX�̏�����������ɍs���܂���ł����B", __FILE__, __FUNCTION__, __LINE__);

		return pGear;
	}

	pGear->_pGear->SetCamera(view);
	// �e�N�X�`���̓ǂݍ���
	//pGear->_pGear->SetTextureDirectoryName("texture");
	
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
	
	child->_pParent = parentGear;

	return;
}

/*
	�M�A�̕`��p
	�����ł���gear��`�悵�A���̂��Ƃ���gear�Ɏq��������΂��̕`�������
*/
void CharaEntity::mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){

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
void CharaEntity::mBodyGearRotation(std::shared_ptr<Gear> gear, const Vector3 rotation){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear)return;

	if (gear->_pParent)
	{
	
		auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
		auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

		gear->_pGear->property._transform._rotation = gearRotation + gear->_parentDifference._rotation;

		Matrix4x4 rotationMatrix;
		rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
		Vector3 position = gear->_parentDifference._translation;
		position = position.TransformCoordNormal(rotationMatrix);

		gear->_pGear->property._transform._translation = gearTranslation + position;
	}
	gear->_pGear->property._transform._rotation += rotation;

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mBodyGearRotation(child, rotation);
	}


	return;
}


//
void CharaEntity::mPartsGearRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear || !gear->_pParent)return;

	auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
	auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

	gear->_pGear->property._transform._rotation = gearRotation + gear->_parentDifference._rotation;

	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
	Vector3 position = gear->_parentDifference._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	gear->_pGear->property._transform._translation = gearTranslation + position;

	gear->_parentDifference._rotation += rotation;

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mPartsGearRotation(child, rotation);
	}

	return;
}


Transform CharaEntity::mGetTransformInterpolation(Transform first, Transform last, const int allFrame, const int nowFrame){
	Transform output;

	// �ړ�
	output._translation = Interpolation<Vector3>(first._translation, last._translation, allFrame, nowFrame);

	// ��]
	output._rotation = Interpolation<Vector3>(first._rotation, last._rotation, allFrame, nowFrame);

	// �g��k��
	output._scale = Interpolation<Vector3>(first._scale, last._scale, allFrame, nowFrame);

	return output;
}


/*
�M�A�����I�u�W�F�N�g�̉�]�p
�̑S�̂��ψ�ɉ�]����
�d�g�݂�mGearRender�ƈꏏ
�L�[�t���[���A�j���[�V�����p
*/
void CharaEntity::mBodyGearKeyframeRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear)return;

	if (gear->_pParent)
	{

		auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
		auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

		gear->_pGear->property._transform._rotation = gearRotation + gear->_parentDifference._rotation;

		Matrix4x4 rotationMatrix;
		rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
		Vector3 position = gear->_parentDifference._translation;
		position = position.TransformCoordNormal(rotationMatrix);

		gear->_pGear->property._transform._translation = gearTranslation + position;
	}
	gear->_pGear->property._transform._rotation = rotation;

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mBodyGearKeyframeRotation(child, rotation);
	}


}
/*
�M�A�����I�u�W�F�N�g�̉�]�p
������������]�ł���
�d�g�݂�mGearRender�ƈꏏ
�L�[�t���[���A�j���[�V�����p
*/
void CharaEntity::mPartsGearKeyframeRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear || !gear->_pParent)return;

	auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
	auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

	gear->_pGear->property._transform._rotation = gearRotation + gear->_parentDifference._rotation;

	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
	Vector3 position = gear->_parentDifference._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	gear->_pGear->property._transform._translation = gearTranslation + position;

	gear->_parentDifference._rotation = rotation;

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mPartsGearKeyframeRotation(child, rotation);
	}
}


/*
�M�A�����I�u�W�F�N�g�̈ړ��p
�d�g�݂�mGearRender�ƈꏏ
*/
void CharaEntity::mGearKeyframeTranslation(std::shared_ptr<Gear> gear, aetherClass::Vector3 move){
	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear)return;

	// �M�A�Ƃ��̃R���C�_�[�𓮂���
	gear->_pGear->property._transform._translation = move+gear->_topDifference._translation;
	gear->_pGear->property._transform._translation = move + gear->_topDifference._translation;

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mGearKeyframeTranslation(child, move);
	}

}
