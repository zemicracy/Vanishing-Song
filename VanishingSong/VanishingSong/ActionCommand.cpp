#include "ActionCommand.h"
#include <WorldReader.h>
using namespace aetherClass;
ActionCommand::ActionCommand(eMusical type){
	m_type = type;
}
ActionCommand::~ActionCommand(){
	if (m_pSprite)
	{
		m_pSprite->Finalize();
		m_pSprite.reset();
	}
	m_type = eMusical::eNull;
}

eMusical ActionCommand::mGetType(){
	return m_type;
}


aetherClass::SpriteBase::Property& ActionCommand::mGetProperty(){
	return m_pSprite->property;
}

void ActionCommand::mCreate(){

	m_pSprite = std::make_shared<aetherClass::Rectangle2D>();
	m_pSprite->Initialize();
	mOnCreate();
	m_isCall = false;
	m_isEnd = false;
}


void ActionCommand::mReset(){
	mOnReset();
	m_isCall = false;
	m_isEnd = false;
}


void ActionCommand::mRender(aetherClass::ShaderBase *shader){
	m_pSprite->Render(shader);
}
void ActionCommand::mSetTexture(aetherClass::Texture *texture){
	m_pSprite->SetTexture(texture);
}


CharaEntity ActionCommand::mGetCharaEntity(){
	return m_entity;
}

bool ActionCommand::mIsCall(){
	return m_isCall;
}

void ActionCommand::mCallCount(const int count){
	m_callCount = count;
}
int ActionCommand::mCallCount()const{
	return m_callCount;
}


bool ActionCommand::mIsEnd(){
	return m_isEnd;
}

void ActionCommand::mIsEnd(const bool isEnd){
	m_isEnd = isEnd;
}

