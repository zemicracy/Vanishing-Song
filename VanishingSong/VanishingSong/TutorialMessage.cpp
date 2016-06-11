#include "TutorialMessage.h"
#include"WorldReader.h"

using namespace aetherClass;
TutorialMessage::TutorialMessage()
{
	m_isVisible = true;
	m_isShowCover = false;
}


TutorialMessage::~TutorialMessage()
{
	mFinalize();
}
namespace{
	std::shared_ptr<aetherClass::Texture> gCreateTexture(std::string path){
		auto tex = std::make_shared<Texture>();
		tex->Load(path);
		return tex;
	}
}

bool TutorialMessage::mInitialize(){
	m_message = std::make_shared<MessageWindow>();
	m_message->mInitialize();

	m_windowBackCover = std::make_shared<Rectangle2D>();
	m_windowBackCover->Initialize();

	WorldReader reader;
	reader.Load("data\\MessageWindow.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "window"){
			m_windowBackCover->property._transform = itr->_transform;
			m_windowBackCover->property._color = Color(1, 1, 1, 0.5);
			break;
		}
	}

	std::string path = "Texture\\Message\\";
	m_pTextTextureList["Init1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["Init2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["Init3"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstDemoListen1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstDemoListen2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstDemoPerform1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstDemoPerform2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstDemoBattle1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstDemoBattle2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstDemoBattle3"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstPlay1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["FirstPlay2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["AdlibDemoListen1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["AdlibDemoPerform1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["AdlibDemoPerform2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["AdlibDemoBattle1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["AdlibDemoBattle2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["AdlibPlay1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["AdlibPlay2"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["PlayerOnly1"] = gCreateTexture(path + "tmplate.png");
	m_pTextTextureList["Fin1"] = gCreateTexture(path + "tmplate.png");


	m_message->mSetText(m_pTextTextureList["Init1"].get());

	return true;
}

void TutorialMessage::mFinalize(){
	for (auto &itr : m_pTextTextureList){
		itr.second.reset();
	}
	m_pTextTextureList.clear();
	m_message.reset();
}

void TutorialMessage::mShowBackCover(const bool flg){
	m_isShowCover = flg;
}


void TutorialMessage::mRender(aetherClass::ShaderBase* shader, aetherClass::ShaderBase* color){
	if (m_isVisible){
		if (m_isShowCover){
			m_windowBackCover->Render(color);
		}
		m_message->mRender(shader);
	}
}
void TutorialMessage::mUpdate(){
	m_message->mUpdate(false);
}

void TutorialMessage::mSetVisible(const bool flg){
	m_isVisible = flg;
}

bool TutorialMessage::mChangeText(std::string str){
	if (m_pTextTextureList.find(str) == m_pTextTextureList.end())return false;
	m_message->mSetText(m_pTextTextureList.at(str).get());
	return true;
}