#include "OrderList.h"
#include<GameController.h>
#include<WorldReader.h>
#include"ResourceManager.h"
#include<Singleton.h>
#include"ActionSound.h"
#include"Const.h"
#include"GameClock.h"
#include"RhythmManager.h"
#include<iostream>



using namespace aetherClass;
OrderList::OrderList()
{
	m_isAlPlay = 0;
	m_isStart = 0;
	m_isEnd = 0;

	m_isKeyDown = 0;
	m_isPlaySound = 0;

	m_processId = 0;
	m_MaxOrderSize = 0;

	m_isLineStart = 0;
	m_eighterCount = 0;
	m_damagedValue = 0;
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
	m_ActionBoard.reset();

	m_pTextureList.clear();
	GameController::GetJoypad().SetVibration(std::make_pair(0, 0));
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

//初期化
void OrderList::mInitialize(GameManager::eGameMode mode,GameManager::eBattleState& state,std::shared_ptr<ActionBoard> board){
	WorldReader reader;
	std::string dir = "Texture\\OrderList\\";

	m_faze = &state;
	m_mode = mode;
	int requestVal = m_mode == GameManager::eGameMode::eQuarter ? 4 : 8;

	reader.Load("data/orderList.aether");

	
	m_pSpriteList.resize(8);
	m_pSpriteOrigin.resize(8);


	for (auto itr : reader.GetInputWorldInfo()._object){
		itr->_color = Color(0, 0, 0, 1);
		if (itr->_name == "base"){
			gInitializer(m_pBackImage, itr->_transform, itr->_color);
			m_pTextureList[itr->_name] = gLoadTexture(itr->_name, dir + "base.png");
			m_pBackImage->SetTexture(m_pTextureList[itr->_name].get());
			
			m_BackImageOrigin._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
			m_BackImageOrigin._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
			m_BackImageScaleOrigin = itr->_transform._scale;
		}
		if (itr->_name == "linepos"){
			gInitializer(m_pReadLine, itr->_transform, Color(1,0,0,1));
			m_ReadLineOrigin = itr->_transform._translation;
		}

		if (itr->_name == "list1"){
			gInitializer(m_pSpriteList[0], itr->_transform, itr->_color);
			m_pSpriteOrigin[0]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
			m_pSpriteOrigin[0]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
		}
		if (itr->_name == "list3"){
			gInitializer(m_pSpriteList[2], itr->_transform, itr->_color);
			m_pSpriteOrigin[2]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
			m_pSpriteOrigin[2]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
		}
		if (itr->_name == "list5"){

			gInitializer(m_pSpriteList[4], itr->_transform, itr->_color);
			m_pSpriteOrigin[4]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
			m_pSpriteOrigin[4]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
		}
		if (itr->_name == "list7"){
			gInitializer(m_pSpriteList[6], itr->_transform, itr->_color);
			m_pSpriteOrigin[6]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
			m_pSpriteOrigin[6]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
		}
		if (m_mode == GameManager::eGameMode::eEighter){
			//8の時使うよ
			if (itr->_name == "list2"){
				gInitializer(m_pSpriteList[1], itr->_transform, itr->_color);
				m_pSpriteOrigin[1]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
				m_pSpriteOrigin[1]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
			}
			if (itr->_name == "list4"){
				gInitializer(m_pSpriteList[3], itr->_transform, itr->_color);
				m_pSpriteOrigin[3]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
				m_pSpriteOrigin[3]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
			}
			if (itr->_name == "list6"){
				gInitializer(m_pSpriteList[5], itr->_transform, itr->_color);
				m_pSpriteOrigin[5]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
				m_pSpriteOrigin[5]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
			}
			if (itr->_name == "list8"){
				gInitializer(m_pSpriteList[7], itr->_transform, itr->_color);
				m_pSpriteOrigin[7]._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
				m_pSpriteOrigin[7]._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
			}
		}

		if (itr->_name == "volume"){
			gInitializer(m_pVolumeImage, itr->_transform, itr->_color);
			m_VolumeOrigin = itr->_transform._translation + itr->_transform._scale / 2;
			m_VolumeOrigin._z = 0;
			m_pTextureList[itr->_name] = gLoadTexture(itr->_name, dir + "note.png");
			m_pVolumeImage->SetTexture(m_pTextureList[itr->_name].get());
		}
	}

	//ちょうどいいから（
	m_ReadLineOrigin._x = m_pSpriteOrigin[0]._x - (m_pSpriteOrigin[2]._x - m_pSpriteOrigin[0]._x)*3;
	reader.UnLoad();
	m_MaxOrderSize = requestVal;

	m_ActionBoard = board;

	m_playedAction = m_ActionBoard->mGetCommand(eMusical::eNull);

	m_rhythm = &Singleton<RhythmManager>::GetInstance();
//	m_rhythm->mInitializeRhythm(0, 110);
}

void OrderList::mLineUpdate(){
	if (!m_isLineStart) return;
	if(m_rhythm->mIsQuarterBeat()){
		m_eighterCount++;
	}
	{
		float oneWay = m_pSpriteOrigin[2]._x - m_pSpriteOrigin[0]._x;
		float time;
		time = modf(m_rhythm->mQuarterBeatTime(),&time);
		m_pReadLine->property._transform._translation._x = m_ReadLineOrigin._x + (m_eighterCount * oneWay) + (time * oneWay);
	}
	

}

//敵行動フェイズ
void OrderList::mListenUpdate(){
	if (*m_faze != GameManager::eBattleState::eListen) return;
	if (!m_isStart) return;


	if (m_processId > m_MaxOrderSize){
		mListStop();
		m_isAlPlay = true;
		return;
	}	

	m_playedAction = m_ActionBoard->mGetCommand(eMusical::eNull);

	//タイミング	４分と８分
	bool timing = m_mode == GameManager::eGameMode::eQuarter ? m_rhythm->mIsQuarterBeat() : m_rhythm->mIsEighterBeat();

	if (timing){
		if (m_processId >= m_MaxOrderSize){
			m_processId++;
			return;
		};
		m_playedAction = m_EnemyOrderList[m_processId];
		if (m_EnemyOrderList[m_processId]->mGetType() != eMusical::eNull){
			auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_EnemyOrderList[m_processId]->mGetType());
			mPlaySound(sound);
		}
		m_processId++;
	}
}

//プレイヤー入力フェイズ
void OrderList::mPerformUpdate(){
	if (*m_faze != GameManager::eBattleState::ePerform)return;
	if (!m_isStart)return;

	if (!m_isAlPlay){
		//一回しか処理させたくないもの	
		m_PlayerOrderList.clear();
		m_isAlPlay = true;
	}
	float reducation = 0;
	float playtime = m_rhythm->mGetPlayTime();
	if (m_mode == GameManager::eGameMode::eQuarter){
		reducation = 1;
	}
	else if (m_mode == GameManager::eGameMode::eEighter){
		playtime *= 2;
		reducation = 1.2f;
	}

//	printf("time%.2f\n", playtime);

	double magen;
	double exFrame = modf(playtime, &magen);
	auto command = m_ActionBoard->mGetCommand(eMusical::eNull);
	bool onCommand = false;
	m_playedAction = command;

	//タイミング判定
	bool backBeat = m_mode == GameManager::eGameMode::eQuarter ?
		(!m_rhythm->mIsQuarterBeat() && m_rhythm->mIsEighterBeat()) :
		(!m_rhythm->mIsEighterBeat() && m_rhythm->mIsSixteenthBeat()) ;

	
	/*printf("%.3f wholeBeat", m_rhythm->mWholeBeatTime());
	printf("exFrame %.3f\t", exFrame);
	if (m_kGreat*reducation >= exFrame || exFrame >= 1 - m_kGreat*reducation){
		printf("now! ");
	}
	else {
		printf("false ");
	}*/

	//	printf("id: %d\n", m_processId);
		//裏打ちのタイミングで毎回フラグをリセットし次を判定	

	if (backBeat){
			//std::cout << "Reset " << m_processId << std::endl;
		if (m_isKeyDown){
			m_isKeyDown = m_isPlaySound = false;
		}
		else{
			//Debug::mPrint("MISS");
			m_PlayerOrderList.push_back(command);
		}
		m_processId++;
		return;
	}

	{	//キー入力
		auto com = m_ActionBoard->mSelectType();
		if (com){
			command = com;
			onCommand = true;
		}
	}
	if (onCommand){
		m_playedAction = command;
		if (m_isKeyDown) return;

		if (m_kGreat*reducation >= exFrame || exFrame >= 1 - m_kGreat*reducation){
				//Debug::mPrint("Great");
			m_isKeyDown = true;
			m_isPlaySound = true;
			m_PlayerOrderList.push_back(command);
		}
		else {
				//Debug::mPrint("DownMISS");
			m_isKeyDown = true;
			m_PlayerOrderList.push_back(m_ActionBoard->mGetCommand(eMusical::eNull));
			return;
		}
	}



	if (m_processId > m_MaxOrderSize){
		//停止場所	Debug::mPrint("Stop List");
		mListStop();
		return;
	}

	//１つ多く処理して時間調整
	if (m_processId > m_MaxOrderSize - 1)return;


	if (!m_isKeyDown || !m_isPlaySound || command->mGetType() == eMusical::eNull){
		//例外	std::cout << "Failed Keydown\n";
		return;
	}


	//音があるやつは流す
	auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_PlayerOrderList[m_processId]->mGetType());
	sound->mStop();
	sound->mPlaySoundAction(0);
	m_isPlaySound = false;

}

//バトル
void OrderList::mBattleUpdate(){
	if (*m_faze != GameManager::eBattleState::eBattle);
	if (!m_isStart)return;

	if (!m_isAlPlay){
		//一回しか処理させたくないもの	
		m_isAlPlay = true;
		for (int i = 0; i < m_MaxOrderSize; ++i){
			if (m_PlayerOrderList[i]->mGetType() == m_EnemyOrderList[i]->mGetType()){
			}
			else{
				m_PlayerOrderList[i] = m_ActionBoard->mGetCommand(eMusical::eMiss);
			}
		}
	}

	if (m_processId > m_MaxOrderSize){
		mListStop();
		m_isAlPlay = false;
		return;
	}
	m_damagedValue = 0;

	//タイミング	４分と８分
	bool timing = m_mode == GameManager::eGameMode::eQuarter ? m_rhythm->mIsQuarterBeat() : m_rhythm->mIsEighterBeat();

	if (timing){
		//終了準備
		if (m_processId >= m_MaxOrderSize){
			m_processId++;
			return;
		}


		if (m_PlayerOrderList[m_processId]->mGetType() == eMusical::eMiss){
			m_damagedValue = -1;
		}
		else {
			m_damagedValue = 1;
		}



		//アドリブhantei投入場所
		if (m_PlayerOrderList[m_processId]->mGetType() != eMusical::eNull){
			auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_PlayerOrderList[m_processId]->mGetType());
			mPlaySound(sound);
		}
		m_processId++;
	}

}

void OrderList::mUpdate(){

	mLineUpdate();

	switch (*m_faze)
	{
	case GameManager::eBattleState::eListen:
		mListenUpdate();
		break;
	case GameManager::eBattleState::ePerform:
		mPerformUpdate();
		break;
	case GameManager::eBattleState::eBattle:
		mBattleUpdate();
		break;
	default:
		break;
	}

	mRhythmicMotion();
}

void OrderList::mRender(aetherClass::ShaderBase* shader, aetherClass::ShaderBase* debug){
	m_pBackImage->Render(shader);
	m_pVolumeImage->Render(shader);
	const float l_kalpha = 0.5f;

	int requestVal = m_mode == GameManager::eGameMode::eQuarter ? 2 : 1;


	if (*m_faze == GameManager::eBattleState::eListen){
		if (!m_EnemyOrderList.empty()){

			int ifill = 0;
			if (m_processId >= m_EnemyOrderList.size()){
				ifill = m_EnemyOrderList.size();
			}
			else{
				ifill = m_processId;
			}

			for (int i = 0; i < ifill; ++i){
				m_pSpriteList[i*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_EnemyOrderList[i]->mGetType()).get());
				m_pSpriteList[i*requestVal]->property._color._alpha = 1.0f;


				//再生済み透明処理
				if (m_isEnd){
					m_pSpriteList[i*requestVal]->property._color._alpha = l_kalpha;
				}
				else if (m_processId > i + 1 && !m_isAlPlay){
					m_pSpriteList[i*requestVal]->property._color._alpha = l_kalpha;
				}

				m_pSpriteList[i*requestVal]->Render(shader);
			}
		}
	}
	else if (*m_faze == GameManager::eBattleState::ePerform){
		//enemyOrderRender
		for (int i = 0; i < m_EnemyOrderList.size(); ++i){
			m_pSpriteList[i*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_EnemyOrderList[i]->mGetType()).get());
			m_pSpriteList[i*requestVal]->property._color._alpha = l_kalpha;
			m_pSpriteList[i*requestVal]->Render(shader);
		}
		

		if (!m_PlayerOrderList.empty()){
			for (int i = 0; i < m_PlayerOrderList.size(); ++i){
				if (i >= m_MaxOrderSize) break;
				m_pSpriteList[i*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_PlayerOrderList[i]->mGetType()).get());
				m_pSpriteList[i*requestVal]->property._color._alpha = 1.0f;
				m_pSpriteList[i*requestVal]->Render(shader);
			}
		}

	}
	else if (*m_faze == GameManager::eBattleState::eBattle){
		for (int i = m_processId; i < m_PlayerOrderList.size(); ++i){
			if (i >= m_MaxOrderSize) break;
			m_pSpriteList[i*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_PlayerOrderList[i]->mGetType()).get());
			m_pSpriteList[i*requestVal]->property._color._alpha = 1.0f;
			m_pSpriteList[i*requestVal]->Render(shader);
		}
	}

	if (m_isLineStart){
		m_pReadLine->Render(debug);
	}
}

std::shared_ptr<ActionCommand> OrderList::mGetActionCommand(){
	return m_playedAction;
}

void OrderList::mAddEnemyOrder(std::vector<std::shared_ptr<ActionCommand>>& input){
	if (m_isStart)return;
	m_EnemyOrderList.clear();

	int requestVal = m_mode == GameManager::eGameMode::eQuarter ? 4 : 8;

	m_EnemyOrderList.reserve(requestVal);
	for (int i = 0; i < requestVal; ++i){
		m_EnemyOrderList.push_back(input[i]);
	}
}

void OrderList::mPlay(){
	m_pBackImage->property._color._red = 1;
	m_isAlPlay = false;
	m_isStart = true;
	m_isEnd = false;
	m_processId = 0;
	m_damagedValue = 0;
	m_playedAction = m_ActionBoard->mGetCommand(eMusical::eNull);

}

void OrderList::mListStop(){
	m_pBackImage->property._color._red = 0.0;
	m_isStart = false;
	m_isKeyDown = false;
	m_isEnd = true;
	m_processId = 0;
	m_playedAction = m_ActionBoard->mGetCommand(eMusical::eNull);
	m_isLineStart = false;
	m_pReadLine->property._transform._translation = m_ReadLineOrigin;
	m_eighterCount = 0;
	m_damagedValue = 0;
}
void OrderList::mLinePlay(){
	m_isLineStart = true;
	m_pReadLine->property._transform._translation = m_ReadLineOrigin;
	m_eighterCount = 0;
}

void OrderList::mEndReset(){
	mListStop();
	m_EnemyOrderList.clear();
	m_PlayerOrderList.clear();

}

void OrderList::mPlaySound(std::shared_ptr<ActionSound> sound){
	sound->mStop();
	sound->mPlaySoundAction(-100);
}

void OrderList::mRhythmicMotion(){
	//BPMから1フレームの変化量を計算
	//float note = m_mode == GameManager::eGameMode::eQuarter ? 360 * m_rhythm->mQuarterBeatTime() : 360 * m_rhythm->mEighterBeatTime();
	float note = 360 * m_rhythm->mQuarterBeatTime();
	float nowFrameWave = cos(note * kAetherRadian);
	float scale = nowFrameWave >= 0.8 ? nowFrameWave : 0;

	//拡大縮小をするために
	m_pVolumeImage->property._transform._scale = 150 + (scale * 15);

	{
		auto size = (m_pVolumeImage->property._transform._scale);
		m_pVolumeImage->property._transform._translation = m_VolumeOrigin - (size / 2);
		m_pVolumeImage->property._transform._translation._z = 0;
	}
	
	{
		m_pBackImage->property._transform._scale._x = m_BackImageScaleOrigin._x + (scale*10);
		m_pBackImage->property._transform._scale._y = m_BackImageScaleOrigin._y + (scale*10);
		auto size = (m_pBackImage->property._transform._scale);
		m_pBackImage->property._transform._translation._x = m_BackImageOrigin._x - (size._x / 2);
		m_pBackImage->property._transform._translation._y = m_BackImageOrigin._y - (size._y / 2);
		m_pBackImage->property._transform._translation._z = 0;
	}
	
		for (int i = 0; i < m_pSpriteList.size(); ++i){
			if (!m_pSpriteList.at(i))continue;
			m_pSpriteList[i]->property._transform._scale = 50 + (scale*10);
			auto size = (m_pSpriteList[i]->property._transform._scale);
			m_pSpriteList[i]->property._transform._translation._x = m_pSpriteOrigin[i]._x - (size._x / 2);
			m_pSpriteList[i]->property._transform._translation._y = m_pSpriteOrigin[i]._y - (size._y / 2);
			m_pSpriteList[i]->property._transform._translation._z = 0;
	}

	static int framecnt;
	static int maxFrame;
	static int power = 0;

	if (m_rhythm->mIsWholeBeat()){
		framecnt = 0;
		maxFrame = 2;
		power = 4;
	}
	else if (m_rhythm->mIsQuarterBeat()){
		framecnt = 0;
		maxFrame = 1;
		power = 2;
	}
	if (framecnt < maxFrame){
		GameController::GetJoypad().SetVibration(std::make_pair(15000 * power, 15000 * power));
	}
	else{
		GameController::GetJoypad().SetVibration(std::make_pair(0, 0));
	}
	framecnt++;
}

bool OrderList::mIsEnd(){
	return m_isEnd;
}

int OrderList::mGetDamage(){
	return m_damagedValue;
}