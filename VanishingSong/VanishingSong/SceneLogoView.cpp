#include "SceneLogoView.h"
#include"Const.h"
#include"GameManager.h"
#include"ResourceManager.h"
#include"SceneTitle.h"
using namespace aetherClass;
const std::string SceneLogoView::Name = "Logo";

namespace{
	const int KMaxImage = 38;
}
SceneLogoView::SceneLogoView() : GameScene(Name,GetManager())
{
	mFrameTime = 0;
}


SceneLogoView::~SceneLogoView()
{
	mFrameTime = 0;
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

	mFrameTime = 0;

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

	float vol = GameManager::mGetInstance().mGetVolume();
	m_pSoundDevice = std::make_shared<GameSound>();
	m_pSoundDevice->Load("Sound\\Title\\Logo.wav");
	m_pSoundDevice->SetValume(vol);
	m_pSoundDevice->PlayToOneTime();

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

	
		result = mChangeTexture((int)mFrameTime);
		if (!result){
			ChangeScene(SceneTitle::Name, LoadState::eUse);
		}
		mFrameTime += GameClock::GetDeltaTime() * 20;

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
	return kTransitionEnd;
}
