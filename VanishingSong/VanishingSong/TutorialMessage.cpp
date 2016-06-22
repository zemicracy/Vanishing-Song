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

	

	m_pTexture = nullptr;

	std::string path = "Texture\\Message\\TutorialBattle\\";

	m_pTextTextureList["Init1"] = path + "0.png";
	m_pTextTextureList["Init2"] = path + "1.png";

	m_pTextTextureList["FirstPerform1"] = path + "2.png";
	m_pTextTextureList["FirstBattle1"] = path + "3.png";
	m_pTextTextureList["FirstBattle2"] = path + "4.png";
	m_pTextTextureList["FirstBattle3"] = path + "5.png";
	
	m_pTextTextureList["AdlibListen1"] = path + "6.png";
	m_pTextTextureList["AdlibListen2"] = path + "7.png";
	/*m_pTextTextureList["AdlibPerform1"] = path + "5.png";
	m_pTextTextureList["AdlibPerform2"] = path + "6.png";*/
	m_pTextTextureList["AdlibBattle1"] = path + "8.png";
	m_pTextTextureList["AdlibBattle2"] = path + "9.png";
	
	m_pTextTextureList["PlayerOnly1"] = path + "tmplate.png";
	m_pTextTextureList["Fin1"] = path + "tmplate.png";

	m_pTexture = gCreateTexture(m_pTextTextureList["Init1"]);
	m_message->mSetText(m_pTexture.get());

	m_pButtonTex = gCreateTexture("Texture\\Message\\nextButton.png");
	m_message->mSetButton(m_pButtonTex.get());

	return true;
}

void TutorialMessage::mFinalize(){
	m_pTextTextureList.clear();
	m_message.reset();
	m_pTexture.reset();
	m_pButtonTex.reset();
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
	m_pTexture = gCreateTexture(m_pTextTextureList.at(str));
	m_message->mSetText(m_pTexture.get());
	return true;
}