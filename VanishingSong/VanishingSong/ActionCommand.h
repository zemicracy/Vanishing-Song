#pragma once

#include"CharaStatus.h"
#include"GearFrame.h"
#include "Utility.h"
#include "CharaEntity.h"
#include "Animation.h"
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
	
	void mAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int frameCount);

	CharaEntity mGetCharaEntity();

	/*
	アニメーションの登録
	*/
	void mRegisterAnimation(std::string key ,const int allFrame, std::string first, std::string last);
	
	/*
		アニメーションの値取得用
	*/
	AnimationFrame mGetAnimation(std::string key);

	void mCallCount(const int);
	int mCallCount()const;

	bool mIsEnd();

protected:
	void mIsEnd(const bool);
private:
	virtual void mOnCreate() = 0;
	virtual void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, float timeScale, int frameCount) = 0;
	virtual void mOnReset() = 0;
	

private:
	eMusical m_type;
	std::shared_ptr<aetherClass::Rectangle2D>m_pSprite;
	CharaEntity m_entity;

	std::unordered_map<std::string, AnimationFrame> m_animation;
	int m_callCount;
	bool m_isCall;
	bool m_isEnd;
};

