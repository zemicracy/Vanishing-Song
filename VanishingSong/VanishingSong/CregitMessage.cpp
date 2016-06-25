#include "CregitMessage.h"

using namespace aetherClass;
CregitMessage::CregitMessage()
{
	mInitialize();
}


CregitMessage::~CregitMessage()
{
	mFinalize();
}


namespace{
	std::shared_ptr<aetherClass::Texture> gCreateTexture(std::string path){
		auto tex = std::make_shared<aetherClass::Texture>();
		tex->Load(path);
		return tex;
	}
}
void CregitMessage::mInitialize(){
	m_pSprite = std::make_shared<Rectangle2D>();
	m_pSprite->Initialize();
	m_pSprite->property._transform._scale = Vector3(1280, 720, 0);

	std::string folder = "Texture\\Cregit\\";
	m_textureList.insert(std::make_pair("1", gCreateTexture(folder + "cregit1.png")));
	m_textureList.insert(std::make_pair("2", gCreateTexture(folder + "cregit2.png")));
	m_textureList.insert(std::make_pair("3", gCreateTexture(folder + "cregit3.png")));
	m_textureList.insert(std::make_pair("4", gCreateTexture(folder + "cregit4.png")));
	m_textureList.insert(std::make_pair("End", gCreateTexture(folder + "end.png")));


	m_waveMessage.reserve(4);
	m_waveMessage.push_back(gCreateTexture(folder+"programmer.png"));
	m_waveMessage.push_back(gCreateTexture(folder + "designer.png"));
	m_waveMessage.push_back(gCreateTexture(folder + "soundcreater.png"));
	m_waveMessage.push_back(gCreateTexture(folder + "specialthanks.png"));

}
void CregitMessage::mChangeTexture(std::string path){
	if (m_textureList.find(path) == m_textureList.end())return;
	m_pSprite->SetTexture(m_textureList.at(path).get());
}

void CregitMessage::mWaveMessageOpen(UINT index){
	if (index > m_waveMessage.size() - 1)return;
	m_pSprite->SetTexture(m_waveMessage.at(index).get());
}

void CregitMessage::mSetActive(bool flg){
	m_active = flg;
}

void CregitMessage::mUpdate(float timeScale){
	if (!m_active){
		m_pSprite->property._color._alpha -= 0.1*timeScale;
		if (m_pSprite->property._color._alpha < 0){
			m_pSprite->property._color._alpha = 0;
		}
	}
	else{
		m_pSprite->property._color._alpha += 0.1*timeScale;
		if (m_pSprite->property._color._alpha > 1){
			m_pSprite->property._color._alpha = 1;
		}
	}

}

void CregitMessage::mRender(aetherClass::ShaderBase* shader){
	m_pSprite->Render(shader);

}
void CregitMessage::mFinalize(){
	m_pSprite->Finalize();
	for (auto &itr : m_textureList){
		itr.second.reset();
	}
	for (auto &itr : m_waveMessage){
		itr.reset();
	}
	m_textureList.clear();
	m_waveMessage.clear();
}

