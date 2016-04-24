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
	ActionCommand(eCommandType);
	virtual ~ActionCommand();

	eCommandType mGetType();
	aetherClass::SpriteBase::Property& mGetProperty();
	void mCreate();
	bool mIsCall();
	void mReset();
	void mRender(aetherClass::ShaderBase*);
	void mSetTexture(aetherClass::Texture*);
	
	void mAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>, float timeScale, int frameCount);

	CharaEntity mGetCharaEntity();

	/*
	アニメーションの登録
	*/
	void mRegisterAnimation(const int allFrame, std::string first, std::string last);
	
	/*
		アニメーションの値取得用
	*/
	AnimationFrame mGetAnimation();

	void mCallCount(const int);
	int mCallCount()const;
private:
	virtual void mOnCreate() = 0;
	virtual void mOnAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>, float timeScale, int frameCount) = 0;
private:
	eCommandType m_type;
	std::shared_ptr<aetherClass::Rectangle2D>m_pSprite;
	CharaEntity m_entity;

	AnimationFrame m_animation;
	AnimationFrame m_defaultAnimation;   // 基本的なアニメーションの値を含んだ連想配列
	int m_callCount;
	bool m_isCall;
};

