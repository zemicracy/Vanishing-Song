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

	m_enemyDamageCounter = 0;
	m_playerDamageCounter = 0;
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

//������
void OrderList::mInitialize(GameManager::eGameMode mode,GameManager::eBattleState& state){
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
			//8�̎��g����
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
			m_pTextureList[itr->_name] = gLoadTexture(itr->_name, dir + "heart.png");
			m_pVolumeImage->SetTexture(m_pTextureList[itr->_name].get());
		}
	}


	reader.UnLoad();
	m_MaxOrderSize = requestVal;

	m_ActionBoard = std::make_shared<ActionBoard>();
	m_ActionBoard->mInitialize();
	m_rhythm = &Singleton<RhythmManager>::GetInstance();
//	m_rhythm->mInitializeRhythm(0, 110);
}

//�G�s���t�F�C�Y
void OrderList::mListenUpdate(){
	if (*m_faze != GameManager::eBattleState::eListen) return;
	if (!m_isStart) return;


	if (m_processId > m_EnemyOrderList.size()){
		mListStop();
		m_isAlPlay = true;
		Debug::mPrint("END");
	}

	//�^�C�~���O	�S���ƂW��
	bool timing = m_mode == GameManager::eGameMode::eQuarter ? m_rhythm->mIsQuarterBeat() : m_rhythm->mIsEighterBeat();

	if (timing){
		if (m_processId >= m_EnemyOrderList.size()){
			m_processId++;
			return;
		};

		if (m_EnemyOrderList[m_processId]->mGetType() != eMusical::eNull){
			auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_EnemyOrderList[m_processId]->mGetType());
			mPlaySound(sound);
		}
		m_processId++;
	}
}

//�v���C���[���̓t�F�C�Y
void OrderList::mPerformUpdate(){
	if (*m_faze != GameManager::eBattleState::ePerform)return;

	float playtime = m_rhythm->mGetPlayTime();

	if (!m_isAlPlay){
		//��񂵂��������������Ȃ�����	
		m_PlayerOrderList.clear();
	}

	if (m_mode == GameManager::eGameMode::eQuarter){
	}
	else if (m_mode == GameManager::eGameMode::eEighter){
		playtime * 2;
	}

	double magen;
	double exFrame = modf(playtime, &magen);
	auto command = m_ActionBoard->mGetCommand(eMusical::eNull);
	bool onCommand = false;


	{	//�L�[����
		auto com = m_ActionBoard->mSelectType();
		if (com){
			command = com;
			onCommand = true;
		}
	}
	//�^�C�~���O����
	if (onCommand){
		if (m_isKeyDown) return;
		/*printf("exFrame %.2f\n", exFrame);
		printf("id: %d\n\n", m_processId);*/

		if (m_kGreat >= exFrame || exFrame >= 1 - m_kGreat){
				//Debug::mPrint("Great");
			m_isKeyDown = true;
			m_isPlaySound = true;
			m_PlayerOrderList.push_back(command);
		}
		else if (m_kGreat < exFrame && exFrame < 1 - m_kGreat){
				//Debug::mPrint("MISS");
			m_isKeyDown = true;
			m_PlayerOrderList.push_back(m_ActionBoard->mGetCommand(eMusical::eNull));
			return;
		}
	}

	bool backBeat = m_mode == GameManager::eGameMode::eQuarter ?
		(!m_rhythm->mIsQuarterBeat() && m_rhythm->mIsEighterBeat()) :
		(!m_rhythm->mIsEighterBeat() && m_rhythm->mIsSixteenthBeat()) ;

	if (backBeat){
		//���ł��̃^�C�~���O�Ŗ���t���O�����Z�b�g�����𔻒�	
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


	if (m_processId > m_MaxOrderSize - 1){
		//��~�ꏊ	Debug::mPrint("Stop List");
		mListStop();
		return;
	}

	if (!m_isKeyDown || !m_isPlaySound || command->mGetType() == eMusical::eNull){
		//��O	std::cout << "Failed Keydown\n";
		return;
	}

	//���������͗���
	auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_PlayerOrderList[m_processId]->mGetType());
	sound->mStop();
	sound->mPlaySoundAction(0);
	m_isPlaySound = false;

}

//��������
void OrderList::mCheckUpdate(){
	if (*m_faze != GameManager::eBattleState::eCheck)return;
	if (!m_isStart)return;

		if (!m_isAlPlay){
			//��񂵂��������������Ȃ�����	
			m_isAlPlay = true;
			m_enemyDamageCounter = 0;
			m_playerDamageCounter = 0;
		}
		

			//if (m_EnemyOrderList[m_processId]->mGetType())

		//�A�h���uhantei�����ꏊ
		if (m_PlayerOrderList[m_processId]->mGetType() == m_EnemyOrderList[m_processId]->mGetType()){
			m_enemyDamageCounter++;
		}
		else{
			m_playerDamageCounter++;
		}

		m_processId++;
		if (m_processId >= m_PlayerOrderList.size()){
			std::cout << "�G" << m_enemyDamageCounter << std::endl;
			std::cout << "�v���C��" << m_playerDamageCounter << std::endl;
			mListStop();
			m_isAlPlay = false;
			if (m_playerDamageCounter != 0){
				//GameController::GetJoypad().SetVibration(std::make_pair(65535, 65535));
			}
		}
}

//�o�g��
void OrderList::mBattleUpdate(){
	if (*m_faze != GameManager::eBattleState::eBattle);
	if (!m_isStart)return;

		//GameController::GetJoypad().SetVibration(std::make_pair(0, 0));
}

void OrderList::mUpdate(){
	mRhythmicMotion();

	switch (*m_faze)
	{
	case GameManager::eBattleState::eListen:
		mListenUpdate();
		break;
	case GameManager::eBattleState::ePerform:
		mPerformUpdate();
		break;
	case GameManager::eBattleState::eCheck:
		mCheckUpdate();
		break;
	case GameManager::eBattleState::eBattle:
		mBattleUpdate();
		break;
	default:
		break;
	}

}

void OrderList::mRender(aetherClass::ShaderBase* shader, aetherClass::ShaderBase* debug){
	m_pBackImage->Render(shader);
	m_pVolumeImage->Render(shader);

	int requestVal = m_mode == GameManager::eGameMode::eQuarter ? 2 : 1;

	if (*m_faze == GameManager::eBattleState::eListen){
		if (m_EnemyOrderList.empty())return;

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


			//�Đ��ςݓ�������
			if (m_isEnd){
				m_pSpriteList[i*requestVal]->property._color._alpha = 0.2f;
			}
			else if (m_processId > i+1 && !m_isAlPlay){
				m_pSpriteList[i*requestVal]->property._color._alpha = 0.2f;
			}

			m_pSpriteList[i*requestVal]->Render(shader);
		}
	}
	else if (*m_faze == GameManager::eBattleState::ePerform){
		if (m_PlayerOrderList.empty())return;

		for (int i = 0; i < m_PlayerOrderList.size(); ++i){
			if (i > m_MaxOrderSize) break;
			m_pSpriteList[i*requestVal]->SetTexture(m_ActionBoard->mGetCommandTexture(m_PlayerOrderList[i]->mGetType()).get());
			m_pSpriteList[i*requestVal]->Render(shader);
		}

	}
	else if (*m_faze == GameManager::eBattleState::eBattle){

	}
}

std::shared_ptr<ActionCommand> OrderList::mGetActionCommand(){
	return m_PlayerOrderList.at(m_PlayerOrderList.size()-1);
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
}

void OrderList::mListStop(){
	m_pBackImage->property._color._red = 0.5;
	m_isStart = false;
	m_isKeyDown = false;
	m_isEnd = true;
	m_processId = 0;
}

void OrderList::mPlaySound(std::shared_ptr<ActionSound> sound){
	sound->mStop();
	sound->mPlaySoundAction(0);
}

void OrderList::mRhythmicMotion(){
	//BPM����1�t���[���̕ω��ʂ��v�Z
	//float note = m_mode == GameManager::eGameMode::eQuarter ? 360 * m_rhythm->mQuarterBeatTime() : 360 * m_rhythm->mEighterBeatTime();
	float note = 360 * m_rhythm->mQuarterBeatTime();
	float nowFrameWave = cos(note * kAetherRadian);
	float scale = nowFrameWave >= 0.8 ? nowFrameWave : 0;

	//�g��k�������邽�߂�
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
//	else if (m_mode == GameManager::eGameMode::eQuarter ? m_rhythm->mIsQuarterBeat() : m_rhythm->mIsEighterBeat()){
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
