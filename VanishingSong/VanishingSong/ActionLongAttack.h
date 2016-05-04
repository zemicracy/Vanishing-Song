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

	// ���ۂ̎��s����
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, float timeScale, int count)override{
		aetherClass::Transform animationTransform;
		const int allFrame = this->mGetAnimation("aaa")._animation.size();
		for (auto index : this->mGetAnimation("aaa")._animation)
		{
			// ��Ԃ̒l���擾
			animationTransform = this->mGetCharaEntity().mGetTransformInterpolation(index._start, index._end, 5,count);

			// �A�j���[�V�����̓K�p
			if (hash.find(index._name) != hash.end()){
				hash[index._name]->_pGear->property._transform = animationTransform;
			}
		}
	}

	void mOnReset()override{}
};

