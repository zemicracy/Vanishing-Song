#include "OrderList.h"
#include<GameController.h>
#include<WorldReader.h>
#include"ResourceManager.h"
#include<Singleton.h>
#include"ActionSound.h"
#include"Const.h"
#include"GameClock.h"
#include"FragmentShader.h"
#include"RhythmManager.h"


#include "ActionNull.h"
#include"ActionShortAttack.h"
#include"ActionLongAttack.h"
#include"ActionLeftStep.h"
#include"ActionRightStep.h"

#include<iostream>
using namespace aetherClass;
OrderList::OrderList()
{
	m_isStart = false;
	m_isKeyDown = false;
	m_isPlaySound = false;

	m_processId = 0;
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
	m_EnemyOrderList.clear();
	m_PlayerOrderList.clear();
	m_ActionCommand.reset();

	m_pTextureList.clear();
	m_helperSprite.clear();
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
	std::string dir = "Texture\\OrderList\\";


	reader.Load("data/orderList.aether");
	m_pSpriteList.resize(4);
	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "base"){
			gInitializer(m_pBackImage, itr->_transform, itr->_color);
			m_pTextureList[itr->_name] = gLoadTexture(itr->_name, dir + "base.png");
			m_pBackImage->SetTexture(m_pTextureList[itr->_name].get());
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
		if (itr->_name == "volume"){
			gInitializer(m_pVolumeImage, itr->_transform, itr->_color);
			m_volumeOrigin = itr->_transform._translation + itr->_transform._scale / 2;
			m_volumeOrigin._z = 0;
			m_pTextureList[itr->_name] = gLoadTexture(itr->_name, dir + "heart.png");
			m_pVolumeImage->SetTexture(m_pTextureList[itr->_name].get());
//			m_pVolumeImage
		}
	}
	m_helperSprite.resize(4);
	for (int i = 0; i < m_helperSprite.size(); ++i){
		m_helperSprite[i] = std::make_shared<Rectangle2D>();
		m_helperSprite[i]->Initialize();
		m_helperSprite[i]->property._transform._translation._x = i * 80;
		m_helperSprite[i]->property._transform._scale = 50;
		m_helperSprite[i]->property._color = 1;
	}


	reader.UnLoad();
	m_MaxOrderSize = 4;

	m_ActionCommand = std::make_shared<ActionBoard>();
	m_ActionCommand->mInitialize();
	m_rhythm = &Singleton<RhythmManager>::GetInstance();
//	m_rhythm->mInitializeRhythm(0, 110);
	m_mode = eBeatMode::eQuarter;
}

void OrderList::mUpdate(float timeScale){
	m_rhythm->mAcquire();
	mRhythmicMotion();

	//フェーズ開始されてなければ戻る
	if (!m_isStart) return;


	if (m_faze == GameManager::eBattleState::eListen){

		if ( (m_rhythm->mIsQuarterBeat() && m_mode == eBeatMode::eQuarter) || 
			(m_rhythm->mIsEighterBeat() && m_mode == eBeatMode::eEighter) ){
			if (m_EnemyOrderList[m_processId]->mGetType() != eCommandType::eNull){
				auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_EnemyOrderList[m_processId]->mGetType());
				mPlaySound(sound);
			}
			m_processId++;
			if (m_processId >= m_EnemyOrderList.size()-1){
				mListStop();
				m_isAlPlay = true;
			}
		}
	}
	else if (m_faze == GameManager::eBattleState::ePerform){
		float playtime = m_rhythm->mGetPlayTime();
		//int reducationCoefficient = 1;

		if (!m_isAlPlay){
			//一回しか処理させたくないもの	
			m_isAlPlay = true;
			m_PlayerOrderList.clear();
		}

		//再生時間 bpm/sec の仮数部 
		if (m_mode == eBeatMode::eQuarter){
			//reducationCoefficient = 1;
		}
		else if (m_mode == eBeatMode::eEighter){
			playtime * 2;
			//reducationCoefficient = 2;
		}

		double magen;
		double exFrame = modf(playtime, &magen);
		auto command = m_ActionCommand->mGetCommand(7);
		bool onCommand = false;


		{	//キー入力
			auto com = m_ActionCommand->mSelectType();
			if (com){
				command = com;
				onCommand = true;
			}
		}

		//もしコマンドがあれば判断
		if (onCommand){
			if (m_isKeyDown) return;
			printf("exFrame %.2f\n", exFrame);
			printf("id: %d\n\n",m_processId);

			if (m_kGreat >= exFrame || exFrame >= 1 - m_kGreat){
				Debug::mPrint("Great");
				m_isKeyDown = true;
				m_isPlaySound = true;
				m_PlayerOrderList.push_back(command);
			}
			else if (m_kGreat < exFrame && exFrame < 1 - m_kGreat){
				Debug::mPrint("MISS");
				m_isKeyDown = true;
				m_PlayerOrderList.push_back(m_ActionCommand->mGetCommand(7));
				return;
			}
		}



	if (!m_rhythm->mIsQuarterBeat() && m_rhythm->mIsEighterBeat()){
		//裏打ちのタイミングで毎回フラグをリセットし次を判定
		std::cout << "Reset " << m_processId << std::endl;

		if (m_isKeyDown){
			m_isKeyDown = false;
			m_isPlaySound = false;
		}
		else{
			Debug::mPrint("MISS");
			m_PlayerOrderList.push_back(command);
		}
		m_processId++;
		return;
	}


	if (m_processId > m_MaxOrderSize-1){
		//停止場所
		Debug::mPrint("Stop List");
		mListStop();
		return;
	}


	if (!m_isKeyDown || !m_isPlaySound || command->mGetType() == eCommandType::eNull){
		//例外ハジキ
		//std::cout << "Failed Keydown\n";
		return;
	}

	//音があるやつは流す

	std::cout << "Play List\t" << m_isKeyDown <<"\tid:"<< m_processId << std::endl;

	auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_PlayerOrderList[m_processId]->mGetType());
	sound->mStop();
	sound->mPlaySoundAction(0);
	m_isPlaySound = false;

	//次に進める
	//	m_processId++;

	}
	else if (m_faze == GameManager::eBattleState::eCheck){

	}



}

void OrderList::mRender(aetherClass::ShaderBase* shader, aetherClass::ShaderBase* debug){
	m_pBackImage->Render(debug);
	m_pVolumeImage->Render(shader);

	if (m_faze == GameManager::eBattleState::eListen){
		if (m_EnemyOrderList.empty())return;

		int ifill = 0;
		if (m_isAlPlay){
			ifill = m_EnemyOrderList.size()-1;
		}
		else{
			ifill = m_processId;
		}

		for (int i = 0; i < ifill; ++i){
			m_pSpriteList[i]->property._color = m_EnemyOrderList[i]->mGetProperty()._color;
			
			//再生済み透明処理
			if (m_processId > i && !m_isAlPlay){
				m_pSpriteList[i]->property._color._alpha = 0.2f;
			}
			else if (m_isAlPlay){
				m_pSpriteList[i]->property._color._alpha = 0.2f;
			}

			m_pSpriteList[i]->Render(debug);
		}
	}
	else if (m_faze == GameManager::eBattleState::ePerform){
		if (m_PlayerOrderList.empty())return;

		if (m_isAlPlay){
			m_helperSprite[0]->Render(debug);
		}
		if (m_isKeyDown){
			m_helperSprite[1]->Render(debug);
		}
		if (m_isPlaySound){
			m_helperSprite[2]->Render(debug);
		}


		for (int i = 0; i < m_PlayerOrderList.size(); ++i){
			if (i > m_MaxOrderSize) break;
			m_pSpriteList[i]->property._color = m_PlayerOrderList[i]->mGetProperty()._color;
			m_pSpriteList[i]->Render(debug);
		}

	}
}

std::shared_ptr<ActionCommand> OrderList::mGetActionCommand(){
	return std::make_shared<ActionNull>();
}


void OrderList::mAddEnemyOrder(std::vector<std::shared_ptr<ActionCommand>>& input){
	if (m_isStart)return;
	m_EnemyOrderList.clear();
	m_EnemyOrderList.reserve(input.size());
	for (int i = 0; i < input.size(); ++i){
		m_EnemyOrderList.push_back(input[i]);
	}
}

void OrderList::mPlay(){
	m_pBackImage->property._color._red = 1;
	m_isStart = true;
	m_isAlPlay = false;
}

void OrderList::mListStop(){
	m_pBackImage->property._color._red = 0.5;
	m_isStart = false;
	m_isKeyDown = false;
	m_processId = 0;
}

void OrderList::mSetFaze(GameManager::eBattleState state){
	m_faze = state;
}


void OrderList::mPlaySound(std::shared_ptr<ActionSound> sound){
	sound->mStop();
	sound->mPlaySoundAction(0);
}

void OrderList::mRhythmicMotion(){
		//BPMから1フレームの変化量を計算
		float nowFrameWave = cos((360 * (m_rhythm->mGetPlayTime()))*kAetherRadian);
		float scale = nowFrameWave >= 0.8 ? nowFrameWave : 0;

		//拡大縮小をするために
		m_pVolumeImage->property._transform._scale = 150 + (scale * 15);

		auto size = (m_pVolumeImage->property._transform._scale);
		m_pVolumeImage->property._transform._translation = m_volumeOrigin - (size / 2);
		m_pVolumeImage->property._transform._translation._z = 0;
}