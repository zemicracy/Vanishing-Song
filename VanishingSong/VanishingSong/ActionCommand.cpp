#include "ActionCommand.h"


ActionCommand::ActionCommand(eCommandType type){
	m_type = type;
}
ActionCommand::~ActionCommand(){
	m_pSprite->Finalize();
	m_pSprite.reset();
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