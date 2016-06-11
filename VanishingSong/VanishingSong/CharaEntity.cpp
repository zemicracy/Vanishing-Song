#include "CharaEntity.h"
#include "Debug.h"
#include "Const.h"
#include <WorldReader.h>

using namespace aetherClass;

CharaEntity::CharaEntity()
{
}


CharaEntity::~CharaEntity()
{
}




//
void CharaEntity::mFaceToObject(std::shared_ptr<FbxModel>& model, aetherClass::Vector3 facePosition){
	float rad = atan2(facePosition._x - model->property._transform._translation._x, facePosition._z - model->property._transform._translation._z);
	float rotationY = rad / kAetherPI * 180;
	model->property._transform._rotation._y = rotationY;
}


