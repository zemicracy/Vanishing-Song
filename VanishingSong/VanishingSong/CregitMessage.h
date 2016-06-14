#pragma once
#include<Rectangle2D.h>
#include"GameManager.h"
#include<memory>
class CregitMessage
{
public:
	CregitMessage();
	~CregitMessage();

	void mSetActive(bool);
	void mChangeTexture(std::string);
	void mWaveMessageOpen(UINT);
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(float);
private:
	void mInitialize();
	void mFinalize();
private:
	bool m_active;
	std::shared_ptr<aetherClass::SpriteBase>m_pSprite;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_textureList;
	std::vector<std::shared_ptr<aetherClass::Texture>>m_waveMessage;


};

