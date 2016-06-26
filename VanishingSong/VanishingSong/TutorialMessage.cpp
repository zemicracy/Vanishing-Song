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
	std::string gAppend(int key,int index){
		return Tutorial::k_TutorialKey[key] + std::to_string(index);
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

	
	using namespace Tutorial;

	m_pTexture = nullptr;

	std::string path = "Texture\\Message\\TutorialBattle\\";

	m_pTextTextureList[gAppend(0, 1)] = path + "Init1.png";		
	m_pTextTextureList[gAppend(1, 1)] = path + "Listen1.png";
	m_pTextTextureList[gAppend(2, 1)] = path + "Listen2.png";
	m_pTextTextureList[gAppend(3, 1)] = path + "Perform1.png";
	m_pTextTextureList[gAppend(3, 2)] = path + "Perform2.png";
	m_pTextTextureList[gAppend(3, 3)] = path + "Perform3.png";
	m_pTextTextureList[gAppend(4, 1)] = path + "Perform4.png";
	m_pTextTextureList[gAppend(5, 1)] = path + "Battle1.png";
	m_pTextTextureList[gAppend(6, 1)] = path + "Battle2.png";

	m_pTextTextureList[gAppend(7, 1)] = path + "Adlib1.png";
	m_pTextTextureList[gAppend(7, 2)] = path + "Adlib2.png";
	m_pTextTextureList[gAppend(7, 3)] = path + "Adlib3.png";
	m_pTextTextureList[gAppend(7, 4)] = path + "Adlib4.png";
	m_pTextTextureList[gAppend(8, 1)] = path + "Adlib5.png";
	m_pTextTextureList[gAppend(9, 1)] = path + "Adlib6.png";
	m_pTextTextureList[gAppend(9, 2)] = path + "Adlib7.png";

	m_pTextTextureList[gAppend(10, 1)] = path + "Wave1.png";
	m_pTextTextureList[gAppend(11, 1)] = path + "Result.png";

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