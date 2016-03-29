#pragma once
#include <memory>
#include <vector>
#include "FbxModel.h"


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
		aetherClass::FbxModel _pGear;
		std::string _name;

	};
};

