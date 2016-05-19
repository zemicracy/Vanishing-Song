#include "ActionCommand.h"
#include <WorldReader.h>
using namespace aetherClass;
ActionCommand::ActionCommand(eCommandType type){
	m_type = type;
}
ActionCommand::~ActionCommand(){
	if (m_pSprite)
	{
		m_pSprite->Finalize();
		m_pSprite.reset();
	}
	m_type = eCommandType::eNull;
}

eCommandType ActionCommand::mGetType(){
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


void ActionCommand::mAction(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, float timeScale, int frameCount){
	
	mOnAction(hash, timeScale, frameCount);
	m_isCall = true;
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


/*
ÉAÉjÉÅÅ[ÉVÉáÉìÇÃìoò^
*/
void ActionCommand::mRegisterAnimation(std::string key, const int allFrame, std::string first, std::string last){
	bool result = false;
	AnimationFrame animation;
	animation._animation.clear();

	result = mGetCharaEntity().mLoadAnimation(animation._animation, first, last);
	if (!result)
	{
		Debug::mErrorPrint("ì«Ç›çûÇ›é∏îs", __FILE__, __LINE__);
		return;
	}
	animation._animationFrame = allFrame;

	m_animation.insert(std::make_pair(key, animation));
}




AnimationFrame ActionCommand::mGetAnimation(std::string key){
	if (m_animation.find(key) == m_animation.end())return AnimationFrame();
	return m_animation[key];
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

