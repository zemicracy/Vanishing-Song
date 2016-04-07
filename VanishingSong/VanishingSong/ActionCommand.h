#pragma once

#include"CharaStatus.h"
#include"GearFrame.h"

#include<Rectangle2D.h>
class ActionCommand
{
public:
	ActionCommand(eActionType);
	virtual ~ActionCommand();

	eActionType mGetType();
	aetherClass::SpriteBase::Property& mGetProperty();
	void mCreate();

	void mRender(aetherClass::ShaderBase*);
	void mSetTexture(aetherClass::Texture*);

	virtual void mAction(std::shared_ptr<GearFrame>, float timeScale, int count) = 0;
private:
	eActionType m_type;
	std::shared_ptr<aetherClass::Rectangle2D>m_pSprite;

};
