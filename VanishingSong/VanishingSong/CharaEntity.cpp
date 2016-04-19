#include "CharaEntity.h"
#include "Debug.h"
#include "Const.h"
#include "Utility.h"
#include <WorldReader.h>

using namespace aetherClass;

CharaEntity::CharaEntity()
{
}


CharaEntity::~CharaEntity()
{
}

/*
*/
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
	if (!parentGear || !child)return;

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

Transform CharaEntity::mGetTransformInterpolation(Transform first, Transform last, const int allFrame, const int nowFrame){
	Transform output;

	// �ړ�
	output._translation = gInterpolation<Vector3>(first._translation, last._translation, allFrame, nowFrame);

	// ��]
	output._rotation = gInterpolation<Vector3>(first._rotation, last._rotation, allFrame, nowFrame);

	// �g��k��
	output._scale = gInterpolation<Vector3>(first._scale, last._scale, allFrame, nowFrame);

	return output;
}


/*
	���]	
*/
void CharaEntity::mGearRotation(std::shared_ptr<Gear> top, std::shared_ptr<Gear> gear, aetherClass::Vector3 rotation){
	
	if (!gear || !gear->_pGear)return;
	gear->_pGear->property._transform._rotation += rotation;


	if (gear->_pParent)
	{
		// �ŏ�ʂƂ̍����X�V
		gear->_topDifference._translation = gear->_pGear->property._transform._translation -top->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - top->_pGear->property._transform._rotation;

		auto gearRotation = top->_pGear->property._transform._rotation;
		auto gearTranslation = top->_pGear->property._transform._translation;

		Matrix4x4 rotationMatrix;
		Vector3 rotationY = Vector3(0, gearRotation._y, 0);
		rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

		Vector3 position = gear->_topDifference._translation;
		position = position.TransformCoordNormal(rotationMatrix);
		
		// ���]�ʒu�̌���
		gear->_pGear->property._transform._translation = gearTranslation + position;
	}


	
	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mGearRotation(top,child, rotation);
	}

}

/*
�w��p�[�c�̂݉�]
*/
void CharaEntity::mGearPartsRotation(std::shared_ptr<Gear> top, std::shared_ptr<Gear> gear, Gear::eType notRotaionType, const Vector3 rotation){
	if (!gear || !gear->_pGear)return;


	gear->_pGear->property._transform._rotation += rotation;

	if (gear->_pParent)
	{
		// �ŏ�ʂƂ̍����X�V
		gear->_topDifference._translation = gear->_pGear->property._transform._translation - top->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - top->_pGear->property._transform._rotation;

		auto gearRotation = top->_pGear->property._transform._rotation;
		auto gearTranslation = top->_pGear->property._transform._translation;

		Matrix4x4 rotationMatrix;
		Vector3 rotationY = Vector3(0, gearRotation._y, 0);
		rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

		Vector3 position = gear->_topDifference._translation;
		position = position.TransformCoordNormal(rotationMatrix);

		// ���]�ʒu�̌���
		gear->_pGear->property._transform._translation = gearTranslation + position;
	}
	
	for (auto child : gear->_pChildren)
	{
		if (child->_type == notRotaionType || child->_type == Gear::eType::eNull) continue;
		mGearPartsRotation(top,child,notRotaionType, rotation);
	}
	return;
}



Gear::eType CharaEntity::mSetPartsValue(std::string partsName, Transform* input, Transform value){
	/*	��	*/
	if (partsName == "Body"){

		*input = value;
		return Gear::eType::eBody;
	}

	if (partsName == "Waist"){
		*input = value;
		return Gear::eType::eWaist;
	}

	/*	���㔼�g*/
	if (partsName == "LeftUpperArm"){
		*input = value;
		return Gear::eType::eLeftUpperArm;
	}

	if (partsName == "LeftLowerArm"){
		*input = value;
		return Gear::eType::eLeftLowerArm;
	}

	if (partsName == "LeftHand"){
		*input = value;

		return Gear::eType::eLeftHand;
	}

	/*	�E�㔼�g	*/
	if (partsName == "RightUpperArm"){
		*input = value;
		return Gear::eType::eRightUpperArm;
	}

	if (partsName == "RightLowerArm"){
		*input = value;
		return Gear::eType::eRightLowerArm;
	}

	if (partsName == "RightHand"){
		*input = value;
		return Gear::eType::eRightHand;
	}

	/*	�E��	*/
	if (partsName == "RightUpperLeg"){
		*input = value;
		return Gear::eType::eRightUpperLeg;
	}

	if (partsName == "RightLowerLeg"){
		*input = value;
		return Gear::eType::eRightLowerLeg;
	}

	/*	����	*/
	if (partsName == "LeftUpperLeg"){
		*input = value;
		return Gear::eType::eLeftUpperLeg;
	}

	if (partsName == "LeftLowerLeg"){
		*input = value;
		return Gear::eType::eLeftLowerLeg;
	}

	return Gear::eType::eNull;
}

/*
�L�[�t���[���A�j���[�V������ǂݍ��ނ悤
*/
bool CharaEntity::mLoadAnimation(std::vector<Animation>&animationVector,std::string startState, std::string endState){
	WorldReader read;
	bool result;
	result = read.Load(startState);
	if (!result)
	{
		return false;
	}
	Animation animation;
	for (auto index : read.GetInputWorldInfo()._object)
	{
		animation._name = mSetPartsValue(index->_name, &animation._start, index->_transform);
		animationVector.push_back(animation);
	}
	read.UnLoad();

	result = read.Load(endState);
	if (!result)
	{
		return false;
	}

	for (auto index : read.GetInputWorldInfo()._object)
	{
		for (auto& endIndex : animationVector)
		{
			Gear::eType type;
			type = mSetPartsValue(index->_name, &animation._end, index->_transform);

			if (endIndex._name == type)
			{
				endIndex._end = animation._end;
			}
		}
	}

	read.UnLoad();

	return true;
}