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

	_pParent.reset();
	
	// Žq‹Ÿ”z—ñ‚Ì—v‘fíœ
	for (auto index : _pChild)
	{
		if (!index)continue;
		index.reset();
		index = nullptr;
	}

	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.release();
	}

	if (_pColider)
	{
		_pColider->Finalize();
		_pColider.release();
	}
	
	_name.clear();

}