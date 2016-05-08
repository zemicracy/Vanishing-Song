#pragma once
#include "ActionCommand.h"
class ActionExHeel :
	public ActionCommand
{
public:
	ActionExHeel() :ActionCommand(eCommandType::eSkill){}
	~ActionExHeel(){}

	void mOnCreate()override{
		this->mRegisterAnimation("start", 50, "data\\Player\\LongAttack\\Stanby.aether", "data\\Player\\LongAttack\\Shot.aether");
		m_baseUseMP = 5;
	}
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, float timeScale, int count)override{
		aetherClass::Transform animationTransform;
		const int allFrame = this->mGetAnimation("start")._animationFrame;
		for (auto index : this->mGetAnimation("start")._animation)
		{
			// 補間の値を取得
			animationTransform = this->mGetCharaEntity().mGetTransformInterpolation(index._start, index._end, 5, count);

			// アニメーションの適用
			if (hash.find(index._name) != hash.end()){
				hash[index._name]->_pGear->property._transform = animationTransform;
			}
		}


		if (count >= allFrame){
			mIsEnd(true);
		}
		return;
	
	}

	void mOnReset()override{}
};

