#include "Gear.h"
#include "Utility.h"

Gear::Gear()
{
}


Gear::~Gear()
{
	Release();
}

void Gear::Release(){

	// �e������΍폜
	if (_pParent)
	{
		_pParent->Release();
		_pParent.reset();
		_pParent = nullptr;
	}

	// �q���z��̗v�f�폜
	for (auto index : _pChildren)
	{
		if (!index)continue;
		index->Release();
		index.reset();
		index = nullptr;
	}

	// �M�A�̉��
	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.reset();
		_pGear = nullptr;
	}

	// �R���C�_�[�̉��
	if (_pColider)
	{
		_pColider->Finalize();
		_pColider.reset();
		_pColider = nullptr;
	}
	
	_type = eType::eNull;

}