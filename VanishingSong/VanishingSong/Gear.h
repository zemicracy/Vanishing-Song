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
		// �̂̃p�[�c
		eBody,
		
		// �r�p�[�c
		eRightUpperArm,
		eLeftUpperArm,
		eRightLowerArm,
		eLeftLowerArm,

		// ���p�[�c
		eWaist,

		// ���p�[�c
		eRightUpperLeg,
		eLeftUpperLeg,
		eRightLowerLeg,
		eLeftLowerLeg,

		// ��̃p�[�c
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

