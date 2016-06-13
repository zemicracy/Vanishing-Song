#pragma once

#include"CharaStatus.h"
#include "CharaEntity.h"
#include<Rectangle2D.h>
#include <WorldReader.h>

class ActionCommand
{

public:
	ActionCommand(eMusical);
	virtual ~ActionCommand();

	eMusical mGetType();
	aetherClass::SpriteBase::Property& mGetProperty();
	void mCreate();
	bool mIsCall();
	void mReset();
	void mRender(aetherClass::ShaderBase*);
	void mSetTexture(aetherClass::Texture*);
	

	CharaEntity mGetCharaEntity();

	void mCallCount(const int);
	int mCallCount()const;

	bool mIsEnd();

protected:
	void mIsEnd(const bool);
private:
	virtual void mOnCreate() = 0;
	virtual void mOnReset() = 0;
	

private:
	eMusical m_type;
	std::shared_ptr<aetherClass::Rectangle2D>m_pSprite;
	CharaEntity m_entity;

	int m_callCount;
	bool m_isCall;
	bool m_isEnd;
};

