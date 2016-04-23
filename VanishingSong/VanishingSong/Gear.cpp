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

	// 親がいれば削除
	if (_pParent)
	{
		_pParent->Release();
		_pParent.reset();
		_pParent = nullptr;
	}

	// 配列をきれいにする
	_pChildren.clear();

	// ギアの解放
	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.reset();
		_pGear = nullptr;
	}
	_type = eType::eNull;

}