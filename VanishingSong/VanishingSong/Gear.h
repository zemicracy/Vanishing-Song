#pragma once
#include <memory>
#include <vector>
#include "FbxModel.h"
#include "ModelBase.h"
#include "Cube.h"
#include <Matrix4x4.h>

class Gear
{
public:

	enum  class eType
	{
		// 体のパーツ
		eBody,
		
		// 腕パーツ
		eRightUpperArm,
		eLeftUpperArm,
		eRightLowerArm,
		eLeftLowerArm,

		// 腰パーツ
		eWaist,

		// 足パーツ
		eRightUpperLeg,
		eLeftUpperLeg,
		eRightLowerLeg,
		eLeftLowerLeg,

		// 手のパーツ
		eRightHand,
		eLeftHand,
		eNull,
	};

	Gear();
	~Gear();
	void Release();
public:


		std::shared_ptr<Gear> _pParent;
		std::vector<std::shared_ptr<Gear>> _pChildren;
		std::shared_ptr<aetherClass::FbxModel> _pGear;
		eType _type;
		aetherClass::Transform _parentDifference;
		aetherClass::Transform _topDifference;
};

