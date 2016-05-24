#include "BattleMessage.h"

using namespace aetherClass;
BattleMessage::BattleMessage()
{
	mInitialize();
}


BattleMessage::~BattleMessage()
{
	mFinalize();
}

void BattleMessage::mInitialize(){
	m_pSprite = std::make_shared<Rectangle2D>();
	m_pSprite->Initialize();
	m_pSprite->property._transform._scale = Vector3(1280, 720, 0);

	m_textureList.insert(std::make_pair(GameManager::eBattleState::eListen, std::make_shared<Texture>()));
	m_textureList.insert(std::make_pair(GameManager::eBattleState::ePerform, std::make_shared<Texture>()));
	m_textureList.insert(std::make_pair(GameManager::eBattleState::eBattle, std::make_shared<Texture>()));
	
	std::string folder = "Texture\\Battle\\";
	m_textureList.at(GameManager::eBattleState::eListen)->Load(folder + "listen.png");
	m_textureList.at(GameManager::eBattleState::eBattle)->Load(folder + "battle.png");
	m_textureList.at(GameManager::eBattleState::ePerform)->Load(folder + "perform.png");

}
void BattleMessage::mChangeTexture(GameManager::eBattleState state){
	if (m_textureList.find(state) == m_textureList.end())return;
	m_pSprite->SetTexture(m_textureList.at(state).get());

}

void BattleMessage::mSetActive(bool flg){
	if (m_active){
		m_pSprite->property._color._alpha = 1;
	}
	m_active = flg;
}

void BattleMessage::mRender(aetherClass::ShaderBase* shader){
	if (!m_active){
		m_pSprite->property._color._alpha -= 0.1;
	}

	m_pSprite->Render(shader);
	
}
void BattleMessage::mFinalize(){
	m_pSprite->Finalize();
}

