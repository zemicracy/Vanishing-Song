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




std::vector<Animation>& ActionCommand::GetAnimationValue(){
	return m_animation;
}





void ActionCommand::CallCount(const int count){
	m_callCount = count;
}
int ActionCommand::CallCount()const{
	return m_callCount;
}