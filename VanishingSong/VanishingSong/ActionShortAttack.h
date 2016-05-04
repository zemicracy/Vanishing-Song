#pragma once
#include "ActionCommand.h"

class ActionShortAttack :
	public ActionCommand
{
private:
	int m_frame;
	int m_count;
	std::array<std::string,4> m_animationName;
public:
	ActionShortAttack():ActionCommand(eCommandType::eShortDistanceAttack){}
	~ActionShortAttack(){}
	void mOnCreate()override{
		this->mRegisterAnimation("one", 3, "data\\Player\\ShortAttack\\Attack_Stay.aether", "data\\Player\\ShortAttack\\Attack_Break1.aether");
		this->mRegisterAnimation("two", 3, "data\\Player\\ShortAttack\\Attack_Break1.aether", "data\\Player\\ShortAttack\\Attack_Break2.aether");
		this->mRegisterAnimation("three", 3, "data\\Player\\ShortAttack\\Attack_Break2.aether", "data\\Player\\ShortAttack\\Attack_Break3.aether");
		this->mRegisterAnimation("four", 3, "data\\Player\\ShortAttack\\Attack_Break3.aether", "data\\Player\\ShortAttack\\Attack_Break4.aether");

		m_animationName[0] = "one";
		m_animationName[1] = "two";
		m_animationName[2] = "three";
		m_animationName[3] = "four";
		m_frame = 0;
		m_count = 0;
		m_baseUseMP = 5;
	}

	//
	void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, float timeScale, int count)override{
		aetherClass::Transform animationTransform;
		const int allFrame = this->mGetAnimation(m_animationName[m_count])._animationFrame;
		for (auto index : this->mGetAnimation(m_animationName[m_count])._animation)
		{
			// 補間の値を取得
			animationTransform = this->mGetCharaEntity().mGetTransformInterpolation(index._start, index._end, allFrame, m_frame);

			// アニメーションの適用
			if (hash.find(index._name) != hash.end()){
				hash[index._name]->_pGear->property._transform = animationTransform;
			}
		}

		if (m_count >= 3 && m_frame >= allFrame){
			mIsEnd(true);
			return;
		}

		if (m_frame >= allFrame){
			m_frame = 0;
			m_count += 1;
			
			return;
		}

		m_frame += 1;
		return;
	}

	void mOnReset()override{
	
		m_frame = 0;
		m_count = 0;
	}

};

