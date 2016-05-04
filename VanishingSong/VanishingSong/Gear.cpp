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

	// ƒMƒA‚Ì‰ð•ú
	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.reset();
		_pGear = nullptr;
	}
	_type = eType::eNull;

}