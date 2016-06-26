#include "SceneEnd.h"
#include "ResourceManager.h"
#include "GameController.h"

using namespace aetherClass;


const std::string SceneEnd::Name = "End";
SceneEnd::SceneEnd() :
GameScene(Name, GetManager())
{
}


SceneEnd::~SceneEnd()
{
}

bool SceneEnd::Initialize(){

	Debug::mPrint("yobareta");

	m_field.mInitialize("Model\\Field\\title_tex");
	m_field.mSetCamera(&m_view);

	eMusical musicalType[] = { eMusical::eBlue, eMusical::eRed, eMusical::eGreen, eMusical::eYellow};
	const int size = 4;
	for (int i = 0; i < size; ++i){
		eMusical type = musicalType[i];
		m_actors[type]._model = ResourceManager::mGetInstance().mGetPlayerHash(type);
		m_actors[type]._model->SetCamera(&m_view);
		m_actors[type]._animationName = "wait";
		m_actors[type]._prevAnimationName = "wait";

	}
	
	m_boss._model = ResourceManager::mGetInstance().mGetEnemyHash(eMusical::eAdlib);
	m_boss._model->SetCamera(&m_view);
	m_boss._animationName = "wait";
	m_boss._prevAnimationName = "wait";

	WorldReader reader;
	reader.Load("data\\End\\PlayerEndScene", true);

	for (auto index : reader.GetInputWorldInfo()._object){
		if (index->_name == "playerBlue"){
			m_actors[eMusical::eBlue]._model->property._transform._translation = index->_transform._translation;
			m_actors[eMusical::eBlue]._model->property._transform._rotation = 180;
		}
		if (index->_name == "playerRed"){
			m_actors[eMusical::eRed]._model->property._transform._translation = index->_transform._translation;
			m_actors[eMusical::eRed]._model->property._transform._rotation = 180;
		}
		if (index->_name == "playerGreen"){
			m_actors[eMusical::eGreen]._model->property._transform._translation = index->_transform._translation;
			m_actors[eMusical::eGreen]._model->property._transform._rotation = 180;
		}
		if (index->_name == "playerYellow"){
			m_actors[eMusical::eYellow]._model->property._transform._translation = index->_transform._translation;
			m_actors[eMusical::eYellow]._model->property._transform._rotation = 180;
		}
		if (index->_name == "boss"){
			m_boss._model->property._transform._translation = index->_transform._translation;
			m_boss._model->property._transform._rotation._y = 180;
		}
	}
	reader.UnLoad();
	for (auto& index : m_actors){
		index.second._model->property._transform._translation._y = 0;
	}
	m_boss._model->property._transform._translation._y = 0;


	m_view.property._rotation = Vector3(-3, 35, 0);
	m_view.property._translation = Vector3(-129, 43, -420);


	m_pSkybox = std::make_unique<Skybox>();
	m_pSkybox->Initialize();
	m_pSkybox->SetCamera(&m_view);
	m_pSkybox->SetTexture(ResourceManager::mGetInstance().GetTexture("skybox").get());

	m_textPathList.clear();
	m_iconPathList.clear();
	m_textPathList.push_back("Texture\\Message\\tmplate.png");
	m_textPathList.push_back("Texture\\Message\\tmplate.png");
	m_textPathList.push_back("Texture\\Message\\tmplate.png");

	m_iconPathList.push_back("Texture\\Icon\\template.png");
	m_iconPathList.push_back("Texture\\Icon\\template.png");
	m_iconPathList.push_back("Texture\\Icon\\template.png");
	
	m_button.Load("Texture\\Message\\nextButton.png");
	m_messageWindow.mInitialize();
	m_messageWindow.mSetButton(&m_button);
	GameManager::mGetInstance().mBossState(GameManager::eBossState::eEnd);
	GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eEnd);
	m_isMessageEnd = false;
	m_isTransitionEnd = false;
	mChangeMessage(m_messageWindow,m_message, 0);
	m_message._count += 1;

	m_returnSE.Load("Sound\\End\\decision.wav");
	const float volume = GameManager::mGetInstance().mGetVolume();
	m_returnSE.SetValume(volume);
	return true;
}

void SceneEnd::Finalize(){
	
}

//
bool SceneEnd::Updater(){

	if (!m_isTransitionEnd)return true;
	bool isPushButton = GameController::GetKey().KeyDownTrigger(VK_SPACE) || GameController::GetJoypad().ButtonPress(eJoyButton::eB);

	if (m_message._count > m_textPathList.size()-1){
		m_isMessageEnd = true;
	}

	if (!m_isMessageEnd&&isPushButton){
		m_returnSE.Stop();
		m_returnSE.PlayToOneTime();
		const int messageCount = m_message._count;
		mChangeMessage(m_messageWindow,m_message, messageCount);
		m_message._count += 1;
	}
	m_messageWindow.mUpdate(false);
	mActorAnimation(m_actors, m_boss);
	return true;
}

void SceneEnd::Render(){
	m_view.Render();
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	m_pSkybox->Render(shaderHash["texture"].get());
	m_field.mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	for (auto& index : m_actors){
		index.second._model->KeyframeAnimationRender(shaderHash["texture"].get());
	}
	m_boss._model->KeyframeAnimationRender(shaderHash["texture"].get());

}

void SceneEnd::UIRender(){
	auto shaderHash = ResourceManager::mGetInstance().mGetShaderHash();
	if (!m_isMessageEnd&&m_isTransitionEnd){
		m_messageWindow.mRender(shaderHash["texture"].get());
	}
	GameManager::mGetInstance().mfadeManager().mRender(shaderHash["color"].get());
}

bool SceneEnd::TransitionIn(){
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		return kTransitionning;
	}
	m_isTransitionEnd = true;
	return true;
}

bool SceneEnd::TransitionOut(){
	if (!GameManager::mGetInstance().mfadeManager().Out(1)){
		return kTransitionning;
	}
	m_isTransitionEnd = true;
	return true;
}

void SceneEnd::mActorAnimation(std::unordered_map<eMusical, EndActor>& actor, EndActor& boss){
	// プレイヤー系
	for (auto& index : actor){
		std::string animationName = index.second._animationName;
		if (animationName != index.second._prevAnimationName){
			index.second._animationCount = NULL;
		}

		if (index.second._model->GetKeyframeCount(animationName) < index.second._animationCount){
			index.second._animationCount = NULL;
		}
		index.second._model->KeyframeUpdate(animationName, index.second._animationCount);
		index.second._animationCount += 1;
	}

	// ボス
	std::string animationName = boss._animationName;
	if (animationName != boss._prevAnimationName){
		boss._animationCount = NULL;
	}

	if (boss._model->GetKeyframeCount(animationName) < boss._animationCount){
		boss._animationCount = NULL;
	}
	boss._model->KeyframeUpdate(animationName, boss._animationCount);
	boss._animationCount += 1;
}

void SceneEnd::mChangeMessage(MessageWindow& messageWindow, Message& message, const int count){
	message._icon = std::make_shared<Texture>();
	message._icon->Load(m_iconPathList.at(count));

	message._text = std::make_shared<Texture>();
	message._text->Load(m_textPathList.at(count));
	messageWindow.mSetIcon(m_message._icon.get());
	messageWindow.mSetText(m_message._text.get());
}