#pragma once
#include"Rectangle2D.h"
#include"GameManager.h"
#include<memory>

//画面サイズの文字を表示するためのクラス

class BattleMessage
{
public:
	BattleMessage();
	~BattleMessage();

	void mSetActive(bool);
	void mChangeTexture(GameManager::eBattleState);
	void mWaveMessageOpen(UINT);
	void mRender(aetherClass::ShaderBase*);
private:
	void mInitialize();
	void mFinalize();
private:
	bool m_active;
	std::shared_ptr<aetherClass::SpriteBase>m_pSprite;
	std::unordered_map<GameManager::eBattleState, std::shared_ptr<aetherClass::Texture>>m_textureList;
	std::vector<std::shared_ptr<aetherClass::Texture>>m_waveMessage;
	
};

