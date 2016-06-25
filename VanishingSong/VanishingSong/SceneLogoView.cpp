#include "SceneLogoView.h"
#include"Const.h"
#include"GameManager.h"
#include"ResourceManager.h"
#include"SceneTitle.h"
#include"GameController.h"
#include<iostream>
using namespace aetherClass;
const std::string SceneLogoView::Name = "Logo";

namespace{
	const int KMaxImage = 54;
}
SceneLogoView::SceneLogoView() : GameScene(Name,GetManager())
{
	m_FrameTime = 0;
}


SceneLogoView::~SceneLogoView()
{
	m_FrameTime = 0;
}

bool SceneLogoView::mChangeTexture(int num){
	bool result = true;
	if (num >= KMaxImage){
		return false;
	}

	m_pSprite->SetTexture(m_pTextureList.at(num).get());
	return result;
}


bool SceneLogoView::Initialize(){
	m_pSprite = std::make_shared<Rectangle2D>();
	m_pSprite->Initialize();
	m_pSprite->property._transform._scale = Vector3(kWindowWidth, kWindowHeight, 0);

	m_FrameTime = 0;
	m_isEndTransition = false;
	m_State = eState::eAether;
	m_PrevState = 0;

	std::string path = "Texture\\Logo\\";
	for (int i = 0; i < KMaxImage; ++i){
		m_pTextureList.insert(std::make_pair(i,std::make_shared<Texture>()));
		if (i < 10){
			m_pTextureList.at(i)->Load(path + "0" + std::to_string(i) + ".png");
		}
		else{
			m_pTextureList.at(i)->Load(path + std::to_string(i) + ".png");
		}
	}

	m_pAetherTexture = std::make_shared<Texture>();
	m_pAetherTexture->Load("Texture\\Logo\\Aether.png");

	float vol = GameManager::mGetInstance().mGetVolume();
	m_pSoundDevice = std::make_shared<GameSound>();
	m_pSoundDevice->Load("Sound\\Title\\Logo.wav");
	m_pSoundDevice->SetValume(vol);

	m_pSprite->SetTexture(m_pAetherTexture.get());

	RegisterScene(new SceneTitle());
	return true;
}

void SceneLogoView::Finalize(){
	if (m_pSprite){
		m_pSprite->Finalize();
		m_pSprite.reset();
	}
	for (auto &itr : m_pTextureList){
		itr.second.reset();
	}
	m_pTextureList.clear();
	if (m_pSoundDevice){
		m_pSoundDevice.reset();
	}
}

//1フレームごとの更新処理
bool SceneLogoView::Updater(){
	bool result = true;
	const bool isPress = GameController::GetKey().KeyDownTrigger(VK_RETURN) || GameController::GetJoypad().ButtonPress(eJoyButton::eStart);

	if (isPress){
		m_PrevState = m_State;
		m_State++;
	}
	
	if (m_State == eState::eAether){
		if (m_FrameTime > 2){
			if (!GameManager::mGetInstance().mfadeManager().In(1)){
				return true;
			}
				m_isEndTransition = false;
				m_PrevState = m_State;
				m_State++;
		}
		else{
			m_FrameTime += (float)GameClock::GetDeltaTime();
		}
	}
	else if (m_State == eState::eZemicracy){
		if (!m_isEndTransition){
			result = mChangeTexture(0);
			if (!GameManager::mGetInstance().mfadeManager().Out(1)){
				return true;
			}
			m_isEndTransition = true;
		}
		if (m_PrevState != m_State){
			m_FrameTime = 0;
			m_pSoundDevice->PlayToOneTime();
			m_PrevState = m_State;
		}

		result = mChangeTexture((int)m_FrameTime);
		if (!result){
			m_State++;
		}
		m_FrameTime += GameClock::GetDeltaTime() * 18;
	}
	else if(m_State == eState::eFin){
		ChangeScene(SceneTitle::Name, LoadState::eUse);
	}
	return true;
}

//1フレームごとの描画処理
void SceneLogoView::Render(){
}

//1フレームごとのui描画処理
void SceneLogoView::UIRender(){
	auto &shaderHash = ResourceManager::mGetInstance().mGetShaderHash();

	m_pSprite->Render(shaderHash["texture"].get());
	GameManager::mGetInstance().mfadeManager().mRender(shaderHash["color"].get());
}

//トランジション効果を設定
bool SceneLogoView::TransitionIn(){
	if (!GameManager::mGetInstance().mfadeManager().In(1)){
		return kTransitionning;
	}
	return kTransitionEnd;
}
bool SceneLogoView::TransitionOut(){
	if (!GameManager::mGetInstance().mfadeManager().Out(1)){
		return kTransitionning;
	}
	m_isEndTransition = true;
	return kTransitionEnd;
}
