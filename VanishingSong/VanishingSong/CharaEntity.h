
#ifndef _CHARAENTITY_H
#define _CHARAENTITY_H

#include <memory>
#include "Const.h"
#include <WorldReader.h>
#include <FbxModel.h>
class CharaEntity
{
public:
	CharaEntity();
	~CharaEntity();
	// �������̃I�u�W�F�N�g�̕���������
	void mFaceToObject(std::shared_ptr<aetherClass::FbxModel>& top, aetherClass::Vector3 facePosition);
};


#endif