#pragma once
#include "ActionCommand.h"
#include <array>
#include "Bullet.h"
namespace{
	const int kMaxBullet = 10;
}
class ActionLongAttack :
	public ActionCommand
{
private:
	
private:

public:
	ActionLongAttack():ActionCommand(eCommandType::eLongDistanceAttack){}
	~ActionLongAttack(){}

	void mOnCreate()override{
		
	}

	// 実際の実行処理
	void mAction(std::shared_ptr<GearFrame>, float timeScale, int count)override{
		
	}
};

