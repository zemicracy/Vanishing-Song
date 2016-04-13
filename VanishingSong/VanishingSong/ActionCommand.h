#pragma once

#include"CharaStatus.h"
#include"GearFrame.h"
#include "Utility.h"
#include "CharaEntity.h"
#include<Rectangle2D.h>
#include <WorldReader.h>
class ActionCommand
{

public:
	struct Animation
	{
		aetherClass::Transform _start;
		aetherClass::Transform _end;
		Gear::eType _name;
	};
public:
	ActionCommand(eCommandType);
	virtual ~ActionCommand();

	eCommandType mGetType();
	aetherClass::SpriteBase::Property& mGetProperty();
	void mCreate();

	void mRender(aetherClass::ShaderBase*);
	void mSetTexture(aetherClass::Texture*);

	virtual void mAction(std::shared_ptr<GearFrame>, float timeScale,int frameCount) = 0;

	CharaEntity mGetCharaEntity();

	/*
		アニメーションの値セット用
	*/
	bool LoadAnimation(std::string startState,std::string endState);

	/*
		アニメーションの値取得用
	*/
	std::vector<Animation>& GetAnimationValue();

	void CallCount(const int);
	int CallCount()const;
private:
	Gear::eType SetPartsValue(std::string,aetherClass::Transform* input,aetherClass::Transform value);

private:
	eCommandType m_type;
	std::shared_ptr<aetherClass::Rectangle2D>m_pSprite;
	CharaEntity m_entity;

	std::vector<Animation> m_animation;

	int m_callCount;
};

