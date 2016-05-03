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

	// �M�A�̉��
	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.reset();
		_pGear = nullptr;
	}
	_type = eType::eNull;

}