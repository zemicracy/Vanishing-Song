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

	// �z������ꂢ�ɂ���
	_pChildren.clear();

	// �M�A�̉��
	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.reset();
		_pGear = nullptr;
	}
	_type = eType::eNull;

}