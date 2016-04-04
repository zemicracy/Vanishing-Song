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

	// 子供配列の要素削除
	for (auto index : _pChildren)
	{
		if (!index)continue;
		index->Release();
		index.reset();
		index = nullptr;
	}

	// ギアの解放
	if (_pGear)
	{
		_pGear->Finalize();
		_pGear.reset();
		_pGear = nullptr;
	}

	// コライダーの解放
	if (_pColider)
	{
		_pColider->Finalize();
		_pColider.reset();
		_pColider = nullptr;
	}
	
	_type = eType::eNull;

}