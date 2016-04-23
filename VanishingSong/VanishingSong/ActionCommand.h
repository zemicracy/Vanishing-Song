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
		�A�j���[�V�����̒l�擾�p
	*/
	std::vector<Animation>& mGetAnimationValue();

	void mCallCount(const int);
	int mCallCount()const;
private:
	virtual void mOnCreate() = 0;
private:
	eCommandType m_type;
	std::shared_ptr<aetherClass::Rectangle2D>m_pSprite;
	CharaEntity m_entity;

	std::vector<Animation> m_animation;

	int m_callCount;
};

