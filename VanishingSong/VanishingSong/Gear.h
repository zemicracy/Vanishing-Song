#pragma once
#include <memory>
#include <vector>
#include "FbxModel.h"
#include "ModelBase.h"
#include "Cube.h"


class Gear
{
public:
	Gear();
	~Gear();
	void Release();


public:

	struct{
		std::shared_ptr<Gear> _pParent;
		std::vector<std::shared_ptr<Gear>> _pChild;
		std::unique_ptr<aetherClass::FbxModel> _pGear;
		std::string _name;
		std::unique_ptr<aetherClass::Cube> _pColider;

	};
};

