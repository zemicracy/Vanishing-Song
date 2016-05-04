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
		this->mRegisterAnimation("aaa",5, "data\\Player\\GunStanby.aether", "data\\Player\\GunGo.aether");
	}

	// 実際の実行処理
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, float timeScale, int count)override{
		aetherClass::Transform animationTransform;
		const int allFrame = this->mGetAnimation("aaa")._animation.size();
		for (auto index : this->mGetAnimation("aaa")._animation)
		{
			// 補間の値を取得
			animationTransform = this->mGetCharaEntity().mGetTransformInterpolation(index._start, index._end, 5,count);

			// アニメーションの適用
			if (hash.find(index._name) != hash.end()){
				hash[index._name]->_pGear->property._transform = animationTransform;
			}
		}
	}

	void mOnReset()override{}
};

