#include "OrderList.h"
#include<GameController.h>
#include<WorldReader.h>
#include"ResourceManager.h"
#include<Singleton.h>
#include"ActionSound.h"
#include"Const.h"
#include"GameClock.h"
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

	m_pReadLine->Finalize();
	m_pReadLine.reset();

	m_pVolumeImage->Finalize();
	m_pVolumeImage.reset();

	m_pFlame->Finalize();
	m_pFlame.reset();

	m_pParticle.release();
	m_playedAction.reset();

	for (auto itr : m_pSpriteList){
		itr->Finalize();
		itr.reset();
	}

	m_EnemyOrderList.clear();
	m_PlayerOrderList.clear();
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
void OrderList::mInitialize(GameManager::eGameMode mode,GameManager::eBattleState& state,ActionBoard* board,BattleField* field,RhythmManager*rhythm){
	WorldReader reader;
	std::string dir = "Texture\\OrderList\\";

	m_faze = &state;
	m_mode = mode;
	m_ActionBoard = board;
	m_Field = field;

	int requestVal = m_mode == GameManager::eGameMode::eQuarter ? 4 : 8;

	reader.Load("data/orderList.aether");

	
	m_pSpriteList.resize(8);
	m_pSpriteOrigin.resize(8);
	

	for (auto& itr : reader.GetInputWorldInfo()._object){
		itr->_color = Color(0, 0, 0, 1);
		if (itr->_name == "base"){
			gInitializer(m_pBackImage, itr->_transform, itr->_color);
			m_pTextureList[itr->_name] = gLoadTexture(itr->_name, dir + "base.png");
			m_pBackImage->SetTexture(m_pTextureList[itr->_name].get());
			
			m_BackImageOrigin._x = itr->_transform._translation._x + itr->_transform._scale._x / 2;
			m_BackImageOrigin._y = itr->_transform._translation._y + itr->_transform._scale._y / 2;
			m_BackImageScaleOrigin = itr->_transform._scale;

			m_BackImageReverceOrigin._x = itr->_transform._translation._x + itr->_transform._scale._x + 80;
			m_BackImageReverceOrigin._y = itr->_transform._translation._y;


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



			m_pTextureList[itr->_name] = gLoadTexture(itr->_name, dir + "note_a.png");
			m_pVolumeImage->SetTexture(m_pTextureList[itr->_name].get());
		}
	}
	reader.UnLoad();

	//ライン開始位置
	m_ReadLineOrigin._x = m_pSpriteOrigin[0]._x - (m_pSpriteOrigin[2]._x - m_pSpriteOrigin[0]._x)*3;
	m_ReadLineReverce._x = m_pSpriteOrigin[7]._x + (m_pSpriteOrigin[2]._x - m_pSpriteOrigin[0]._x) * 3;
	m_MaxOrderSize = requestVal;


	//画面の枠
	m_pFlame = std::make_shared<Rectangle2D>();
	m_pFlame->Initialize();
	m_flameScaleOrigin = m_pFlame->property._transform._scale = Vector3(1280, 720, 1);
	m_flamePosOrigin._x = m_flameScaleOrigin._x / 2;
	m_flamePosOrigin._y = m_flameScaleOrigin._y / 2;

	m_flameScaleOrigin *= 1.2;
	m_pFlame->property._transform._translation = m_flameScaleOrigin / 2 * -1 + m_flameScaleOrigin;



	m_pTextureList["flame"] = gLoadTexture("",dir + "flame.png");
	m_pFlame->SetTexture(m_pTextureList["flame"].get());



	m_playedAction = m_ActionBoard->mGetCommand(eMusical::eNull);

	m_rhythm = rhythm;
//	m_rhythm->mInitializeRhythm(0, 110);

	m_pEffect = std::make_shared<EffectGenerator2D>();
	m_pEffect->mInitialize();

	m_pEffect->mLoadEffect(9, "Break", "Effect\\BreakCommand\\");
	m_pEffect->mLoadEffect(6, "In", "Effect\\InCommand\\");
	m_effectTrans = Transform(Vector3(), Vector3(), Vector3( 16*10, 9*10,0));

	mAppendOptionInit();

//パーティクル
	m_perticleDesc._size = 8;
	m_perticleDesc._scale = 5;
	m_perticleDesc._texturePath = dir + "note_a.png";
	m_perticleDesc._endPoint = Vector3(0, 50, 0);
	m_perticleDesc._rangeMin = Vector3(5, 0, 5);
	m_perticleDesc._rangeMax = Vector3(10, 0, 10);

	m_pParticle = std::make_unique<AttackParticle>(m_perticleDesc, m_Field->mGetCamera());
	m_pParticle->mReset(m_perticleDesc);
}


void OrderList::mAppendOptionInit(){
	m_option = eAppendOption::eNone;
	m_pTextureList["Cover"] = gLoadTexture("", "Texture\\ActionCommand\\Cover.png");
}

void OrderList::mLineUpdate(){
	if (!m_isLineStart) return;
	if (m_rhythm->mIsQuarterBeat()){
		m_eighterCount++;
	}

	if (m_option & eAppendOption::eReverce){
		float oneWay = m_pSpriteOrigin[0]._x - m_pSpriteOrigin[2]._x;
		float time;
		time = modf(m_rhythm->mQuarterBeatTime(), &time);
		m_pReadLine->property._transform._translation._x = m_ReadLineReverce._x + (m_eighterCount * oneWay) + (time * oneWay);
	}
	else{
		float oneWay = m_pSpriteOrigin[2]._x - m_pSpriteOrigin[0]._x;
		float time;
		time = modf(m_rhythm->mQuarterBeatTime(), &time);
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

		//エフェクト関係
		int requestVal = m_mode == GameManager::eGameMode::eQuarter ? 2 : 1;
		if (m_option & eAppendOption::eReverce){
			int id = (m_MaxOrderSize - 1) - m_processId;
			m_effectTrans._translation._x = m_pSpriteOrigin[id*requestVal]._x - m_effectTrans._scale._x / 2;
			m_effectTrans._translation._y = m_pSpriteOrigin[id*requestVal]._y - m_effectTrans._scale._y / 2;
			m_pEffect->mPlay("In", "In" + std::to_string(id), m_effectTrans);
		}
		else{
			m_effectTrans._translation._x = m_pSpriteOrigin[m_processId*requestVal]._x - m_effectTrans._scale._x / 2;
			m_effectTrans._translation._y = m_pSpriteOrigin[m_processId*requestVal]._y - m_effectTrans._scale._y / 2;
			m_pEffect->mPlay("In", "In" + std::to_string(m_processId), m_effectTrans);
		}

		
		m_playedAction = m_EnemyOrderList[m_processId];
		if (m_EnemyOrderList[m_processId]->mGetType() != eMusical::eNull && m_EnemyOrderList[m_processId]->mGetType() != eMusical::eAdlib){
			m_perticleDesc._startPoint = m_Field->mGetEnemyLane(m_EnemyOrderList[m_processId]->mGetType());
			m_pParticle->mReset(m_perticleDesc);

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
		m_PlayerOrderList.reserve(8);
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

	
	//裏打ちのタイミングで毎回フラグをリセットし次を判定
	if (backBeat){
			//std::cout << "Reset " << m_processId << std::endl;
		if (m_isKeyDown){
			m_isKeyDown = m_isPlaySound = false;
		}
		else{
			//Debug::mPrint("MISS");
			if (m_processId < m_MaxOrderSize){
				if (command->mGetType() == m_EnemyOrderList[m_processId]->mGetType()){
				}
				else if (m_EnemyOrderList[m_processId]->mGetType() == eMusical::eAdlib){
					command = m_ActionBoard->mGetCommand(eMusical::eNull);
				}
				else{
					command = m_ActionBoard->mGetCommand(eMusical::eMiss);
				}
			}

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
		//間違ってたらミスを入れる
		if (m_processId >= m_MaxOrderSize)return;
		if (command->mGetType() == m_EnemyOrderList[m_processId]->mGetType()){
		}
		else if (m_EnemyOrderList[m_processId]->mGetType() == eMusical::eAdlib){
		}
		else{
			command = m_ActionBoard->mGetCommand(eMusical::eMiss);
		}


		if (m_kGreat*reducation >= exFrame || exFrame >= 1 - m_kGreat*reducation){
				//Debug::mPrint("Great");
			m_isKeyDown = true;
			m_isPlaySound = true;
			m_PlayerOrderList.push_back(command);
		}
		else {
			//Debug::mPrint("DownMISS");
			m_isKeyDown = true;
			if (m_EnemyOrderList[m_processId]->mGetType() == eMusical::eAdlib){
				m_PlayerOrderList.push_back(m_ActionBoard->mGetCommand(eMusical::eNull));
			}else
			m_PlayerOrderList.push_back(m_ActionBoard->mGetCommand(eMusical::eMiss));
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

	if (m_PlayerOrderList[m_processId]->mGetType() != eMusical::eMiss){
		m_perticleDesc._startPoint = m_Field->mGetPlayerLane(m_PlayerOrderList[m_processId]->mGetType());
		m_pParticle->mReset(m_perticleDesc);
	}
}

//バトル
void OrderList::mBattleUpdate(){
	if (*m_faze != GameManager::eBattleState::eBattle);
	if (!m_isStart)return;

	if (!m_isAlPlay){
		//一回しか処理させたくないもの	
		m_isAlPlay = true;
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
		int requestVal = m_mode == GameManager::eGameMode::eQuarter ? 2 : 1;

		if (m_option & eAppendOption::eReverce){
			int id = (m_MaxOrderSize - 1) - m_processId;
			m_effectTrans._translation._x = m_pSpriteOrigin[id*requestVal]._x - m_effectTrans._scale._x / 2;
			m_effectTrans._translation._y = m_pSpriteOrigin[id*requestVal]._y - m_effectTrans._scale._y / 2;
			m_pEffect->mPlay("Break", "Break" + std::to_string(id), m_effectTrans);
		}
		else{
			m_effectTrans._translation._x = m_pSpriteOrigin[m_processId*requestVal]._x - m_effectTrans._scale._x / 2;
			m_effectTrans._translation._y = m_pSpriteOrigin[m_processId*requestVal]._y - m_effectTrans._scale._y / 2;
			m_pEffect->mPlay("Break", "Break" + std::to_string(m_processId), m_effectTrans);
		}

		if (m_PlayerOrderList[m_processId]->mGetType() == eMusical::eMiss){
			m_damagedValue = -1;
			m_resultData._missCount++;
		}
		else if (m_EnemyOrderList[m_processId]->mGetType() == eMusical::eAdlib && m_PlayerOrderList[m_processId]->mGetType() != eMusical::eNull){
			m_damagedValue = 3;
		}
		else {
			m_damagedValue = 1;
		}

		m_resultData._maxCount++;

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
	m_pEffect->mUpdate(1);

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
	if (m_pParticle){
		m_pParticle->mUpdate(3);
	}
}

void OrderList::mRender(aetherClass::ShaderBase* shader, aetherClass::ShaderBase* debug){
	m_pBackImage->Render(shader);
	m_pFlame->Render(shader);
	m_pVolumeImage->Render(shader);
	m_pEffect->mRender(shader);
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
				//黒塗り
				int id = i;
				if (m_option & eAppendOption::eReverce){
					id = (m_MaxOrderSize -1)- id;
				}

				if (m_option & eAppendOption::eBlack && m_EnemyOrderList[i]->mGetType() != eMusical::eAdlib){
					m_pSpriteList[id*requestVal]->SetTexture(m_pTextureList["Cover"].get());
				}
				else{
					m_pSpriteList[id*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_EnemyOrderList[i]->mGetType()).get());
				}
				m_pSpriteList[id*requestVal]->property._color._alpha = 1.0f;


				//再生済み透明処理
				if (m_isEnd){
					m_pSpriteList[id*requestVal]->property._color._alpha = l_kalpha;
				}
				else{
					if (m_option & eAppendOption::eReverce){
						int revProcess = (m_MaxOrderSize-1) - m_processId;
						if (revProcess < id && !m_isAlPlay){
							m_pSpriteList[id*requestVal]->property._color._alpha = l_kalpha;
						}
					}
					else{
						if (m_processId > id + 1 && !m_isAlPlay){
							m_pSpriteList[id*requestVal]->property._color._alpha = l_kalpha;
						}
					}
				}

				m_pSpriteList[id*requestVal]->Render(shader);
			}
		}
	}
	else if (*m_faze == GameManager::eBattleState::ePerform){
		//enemyOrderRender
		for (int i = m_processId; i < m_EnemyOrderList.size(); ++i){
			int id = i;
			if (m_option & eAppendOption::eReverce){
				id = (m_MaxOrderSize - 1) - id;
			}

			if (i >= m_MaxOrderSize) break;
			if (m_option & 1 && m_EnemyOrderList[i]->mGetType() != eMusical::eAdlib){
				m_pSpriteList[id*requestVal]->SetTexture(m_pTextureList["Cover"].get());
			}
			else{
				m_pSpriteList[id*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_EnemyOrderList[i]->mGetType()).get());
			}
			m_pSpriteList[id*requestVal]->property._color._alpha = l_kalpha;
			m_pSpriteList[id*requestVal]->Render(shader);
		}
		

		if (!m_PlayerOrderList.empty()){
			for (int i = 0; i < m_PlayerOrderList.size(); ++i){
				int id = i;
				if (m_option & eAppendOption::eReverce){
					id = (m_MaxOrderSize - 1) - id;
				}
				if (i >= m_MaxOrderSize) break;
				m_pSpriteList[id*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_PlayerOrderList[i]->mGetType()).get());
				m_pSpriteList[id*requestVal]->property._color._alpha = 1.0f;
				m_pSpriteList[id*requestVal]->Render(shader);
			}
		}

	}
	else if (*m_faze == GameManager::eBattleState::eBattle){
		for (int i = m_processId; i < m_PlayerOrderList.size(); ++i){
			int id = i;
			if (m_option & eAppendOption::eReverce){
				id = (m_MaxOrderSize - 1) - id;
			}
			if (i >= m_MaxOrderSize) break;
			m_pSpriteList[id*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_PlayerOrderList[i]->mGetType()).get());
			m_pSpriteList[id*requestVal]->property._color._alpha = 1.0f;
			m_pSpriteList[id*requestVal]->Render(shader);
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
	if (m_option & eAppendOption::eReverce){
		m_pReadLine->property._transform._translation = m_ReadLineReverce;
	}
	else{
		m_pReadLine->property._transform._translation = m_ReadLineOrigin;
	}
	m_eighterCount = 0;
}

void OrderList::mEndReset(){
	mListStop();
	m_EnemyOrderList.clear();
	m_PlayerOrderList.clear();
	m_option = eAppendOption::eNone;
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

	
	//音符
	{
		m_pVolumeImage->property._transform._scale = 150 + (scale * 15);
		auto size = (m_pVolumeImage->property._transform._scale);
		m_pVolumeImage->property._transform._translation = m_VolumeOrigin - (size / 2);
		m_pVolumeImage->property._transform._translation._z = 0;
	}
	
	//リスト
		if (m_option & eAppendOption::eReverce){
			m_pBackImage->property._transform._scale._x = m_BackImageScaleOrigin._x + (scale * 10);
			m_pBackImage->property._transform._scale._y = m_BackImageScaleOrigin._y + (scale * 10);
			auto size = (m_pBackImage->property._transform._scale);
			m_pBackImage->property._transform._scale._x *= -1;

			float reversive = m_BackImageReverceOrigin._x - (m_BackImageScaleOrigin._x / 2);
			m_pBackImage->property._transform._translation._x = reversive + (size._x / 2);
			m_pBackImage->property._transform._translation._y = m_BackImageOrigin._y - (size._y / 2);
			m_pBackImage->property._transform._translation._z = 0;

		}
		else{
			m_pBackImage->property._transform._scale._x = m_BackImageScaleOrigin._x + (scale * 10);
			m_pBackImage->property._transform._scale._y = m_BackImageScaleOrigin._y + (scale * 10);
			auto size = (m_pBackImage->property._transform._scale);
			m_pBackImage->property._transform._translation._x = m_BackImageOrigin._x - (size._x / 2);
			m_pBackImage->property._transform._translation._y = m_BackImageOrigin._y - (size._y / 2);
			m_pBackImage->property._transform._translation._z = 0;
		}
	//フレーム
	{
		m_pFlame->property._transform._scale._x = m_flameScaleOrigin._x + (scale * 20);
		m_pFlame->property._transform._scale._y = m_flameScaleOrigin._y + (scale * 20);
		auto size = (m_pFlame->property._transform._scale);
		m_pFlame->property._transform._translation._x = m_flamePosOrigin._x - (size._x / 2);
		m_pFlame->property._transform._translation._y = m_flamePosOrigin._y - (size._y / 2);
		m_pFlame->property._transform._translation._z = 0;
	}

	//その他コマンド
		for (int i = 0; i < m_pSpriteList.size(); ++i){
			if (!m_pSpriteList.at(i))continue;
			m_pSpriteList[i]->property._transform._scale = 50 + (scale*10);
			auto size = (m_pSpriteList[i]->property._transform._scale);
			m_pSpriteList[i]->property._transform._translation._x = m_pSpriteOrigin[i]._x - (size._x / 2);
			m_pSpriteList[i]->property._transform._translation._y = m_pSpriteOrigin[i]._y - (size._y / 2);
			m_pSpriteList[i]->property._transform._translation._z = 0;
	}


	//コントローラー
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
		GameController::GetJoypad().SetVibration(std::make_pair(10000 * power, 35000 * power));
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

void OrderList::mRender3D(aetherClass::ShaderBase* shader){
	if (m_pParticle){
		m_pParticle->mRender(shader);
	}
}

ResultData OrderList::mGetResult(){
	return m_resultData;
}

void OrderList::mSetOption(eAppendoption op){
	m_option = op;
}
