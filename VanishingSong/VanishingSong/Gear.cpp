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

	// e‚ª‚¢‚ê‚Îíœ
	if (_pParent)
	{
		_pParent->Release();
		_pParent.reset();
		_pParent = nullptr;
	}

	// ”z—ñ‚ð‚«‚ê‚¢‚É‚·‚é
	_pChildren.clear();

	// ƒMƒA‚Ì‰ð•ú
	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.reset();
		_pGear = nullptr;
	}
	_type = eType::eNull;

}