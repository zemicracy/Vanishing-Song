
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
	// 第二引数のオブジェクトの方向を見る
	void mFaceToObject(std::shared_ptr<aetherClass::FbxModel>& top, aetherClass::Vector3 facePosition);
};


#endif