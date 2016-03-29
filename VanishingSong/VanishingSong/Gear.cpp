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
	ReleaseVector<std::shared_ptr<Gear>>;
	_pGear.Finalize();
	_name.clear();

}