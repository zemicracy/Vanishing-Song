#include "OrderList.h"
#include<GameController.h>
#include<WorldReader.h>
#include"ResourceManager.h"
#include<Singleton.h>
#include"ActionSound.h"
#include"Const.h"
#include"GameClock.h"
#include"FragmentShader.h"

#include "ActionNull.h"
#include "ActionShortAttack.h"
using namespace aetherClass;
OrderList::OrderList()
{
	m_isStart = false;
	m_volume = -20;
	m_bpm = 90;
}


OrderList::~OrderList()
{
	mFinalize();
}
void OrderList::mFinalize(){
	m_pBackImage->Finalize();
	m_pBackImage.reset();

	for (auto itr : m_pSpriteList){
		itr->Finalize();
		itr.reset();
	}
	m_orderList.clear();

	m_listFirst = std::make_shared<ActionNull>();
	m_pTexture.clear();
}

void gInitializer(std::shared_ptr<SpriteBase> &sprite,Transform transform, Color color){
	sprite = std::make_shared<aetherClass::Rectangle2D>();
	sprite->Initialize();
	sprite->property._transform = transform;
	sprite->property._color = color;
}

std::shared_ptr<Texture> gLoadTexture(std::string key, std::string path){
	auto tex = std::make_shared<Texture>();
	tex->Load(path);
	return tex;
}

void OrderList::mInitialize(){
	WorldReader reader;
	std::string dir = "Texture\\";


	reader.Load("data/orderList.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "base"){
			gInitializer(m_pBackImage, itr->_transform, itr->_color);

		}
		if (itr->_name == "list1"){
			gInitializer(m_pSpriteList[0], itr->_transform, itr->_color);
		}
		if (itr->_name == "list2"){
			gInitializer(m_pSpriteList[1], itr->_transform, itr->_color);
		}
		if (itr->_name == "list3"){
			gInitializer(m_pSpriteList[2], itr->_transform, itr->_color);
		}
		if (itr->_name == "list4"){
			gInitializer(m_pSpriteList[3], itr->_transform, itr->_color);
		}
		if (itr->_name == "list5"){
			gInitializer(m_pSpriteList[4], itr->_transform, itr->_color);
		}
		if (itr->_name == "volume"){
			gInitializer(m_pVolumeImage, itr->_transform, itr->_color);
			m_volumeOrigin = itr->_transform._translation + itr->_transform._scale / 2;
			m_volumeOrigin._z = 0;
			m_pTexture[itr->_name] = gLoadTexture(itr->_name, dir + "heart.png");
			m_pVolumeImage->SetTexture(m_pTexture[itr->_name].get());
//			m_pVolumeImage
		}
	}
	reader.UnLoad();

	m_timeRadian = 0;
	m_listFirst = std::make_shared<ActionNull>();
}

void OrderList::mUpdate(float){
	
	//音量変化
	m_volume += GameController::GetMouse().GetWheelMovement() / 10;
	mException();
	{
		//BPMから1フレームの変化量を計算
		float onestep = 60 / m_bpm;
		float timeFrame = 60 * onestep;
		float onceRadius = 360 / timeFrame;
		m_timeRadian += onceRadius * kAetherRadian;
		float scale = int(sin(m_timeRadian)*10) % 10 > 8 ? sin(m_timeRadian) : 0;
		scale *= 6;
		m_pVolumeImage->property._transform._scale = 0.5 * (m_kMaxVolume + m_volume) + 70 + scale;

		auto size = (m_pVolumeImage->property._transform._scale);
		m_pVolumeImage->property._transform._translation = m_volumeOrigin - (size / 2);
		m_pVolumeImage->property._transform._translation._z = 0;
	}
//	Debug::mPrint(std::to_string(GameController::GetMouse().GetWheelMovement()));
//	Debug::mPrint("Volume" + std::to_string(m_volume));


	//リストが空なら
	if (m_orderList.size() == 0){
		return;
	}
	auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_orderList[0]->mGetType());
	if (GameController::GetKey().KeyDownTrigger(VK_SPACE)){
		if (!m_isStart){
			mListPlay();
		}
		else{
			sound->mStop();
			mListStop();
		}
	}

	//再生中の処理
	if (m_isStart){
		m_listFirst = m_orderList[0];

		if (sound->mIsPlayEnd() && m_orderList.size() != 0){
			m_listFirst->mReset();
			m_orderList.erase(m_orderList.begin());
			if (m_orderList.size() == 0){
				mListStop();
				return;
			}
			auto nextSound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_orderList[0]->mGetType());
			nextSound->mPlaySoundAction(m_volume*10);
		}
	}
	
}
void OrderList::mRender(aetherClass::ShaderBase* shader){
	m_pBackImage->Render(shader);
	for (int i = 0; i < m_orderList.size(); ++i){
		m_pSpriteList[i]->property._color = m_orderList[i]->mGetProperty()._color;
		m_pSpriteList[i]->Render(shader);
	}
	static_cast<FragmentShader*>(shader)->_property = FragmentShader::Mode::eTexture;
	m_pVolumeImage->Render(shader);
	static_cast<FragmentShader*>(shader)->_property = FragmentShader::Mode::eColor;
}

std::shared_ptr<ActionCommand> OrderList::mGetActionCommand(){
	return m_listFirst;
}

int OrderList::mGetVolume(){
	return -m_volume;
}

void OrderList::mAddOrder(std::shared_ptr<ActionCommand> input){
	if (m_isStart)return;
	if (m_orderList.size() >= m_kMaxOrderSize) return;
	m_orderList.push_back(input);
}


void OrderList::mListPlay(){
	auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_orderList[0]->mGetType());

	m_pBackImage->property._color._red = 1 - m_pBackImage->property._color._red;
	sound->mPlaySoundAction(m_volume*10);
	m_isStart = true;
}
void OrderList::mListStop(){
	if (m_orderList.size() > 0){
		auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_orderList[0]->mGetType());
		sound->mStop();
	}
	m_orderList.clear();
	m_listFirst = std::make_shared<ActionNull>();
	m_pBackImage->property._color._red = 1 - m_pBackImage->property._color._red;
	m_isStart = false;
}

void OrderList::mException(){
	if (m_volume < -m_kMaxVolume){
		m_volume = -m_kMaxVolume;
	}
	if (m_volume > 0){
		m_volume = 0;
	}
}

void OrderList::mSetBPM(float bpm){
	m_bpm = bpm;
}