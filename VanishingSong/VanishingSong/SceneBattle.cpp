#include "SceneBattle.h"
#include <iostream>
#include<Skybox.h>
#include<Rectangle3D.h>
#include<PixelShader.h>
#include <GameController.h>
#include "ResourceManager.h"

//debug

using namespace aetherClass;
const std::string SceneBattle::Name = "Battle";
SceneBattle::SceneBattle():
GameScene(Name, GetManager())
{
}

SceneBattle::~SceneBattle()
{
}


//interim
std::vector<std::shared_ptr<ActionCommand>>EnemyVector;
eMusical askey[8] = {	eMusical::eBlue, eMusical::eGreen, eMusical::eNull, eMusical::eGreen,
						eMusical::eBlue, eMusical::eYellow, eMusical::eNull, eMusical::eBlue	};

bool SceneBattle::Initialize(){
	Singleton<RhythmManager>::GetInstance().mInitializeRhythm(0, 110);


	m_battleState = GameManager::eBattleState::eListen;

	m_pActionBoard = std::make_shared<ActionBoard>();
	m_pActionBoard->mInitialize();

	m_pOrderList = std::make_unique<OrderList>();
	m_pOrderList->mInitialize(GameManager::eGameMode::eEighter,m_battleState,m_pActionBoard);


	m_rhythm = &Singleton<RhythmManager>::GetInstance();
	m_rhythm->mAcquire();

	m_processState = eGameState::ePreCountIn;
	m_InitUpdateProcess = false;
	m_prevWholeBeatNo = 0;

	for (int i = 0; i < 8; i++){
			EnemyVector.push_back(m_pActionBoard->mGetCommand(askey[i]));
	}

	m_pField = std::make_unique<BattleField>();
	m_pField->mInitialize(&m_view);

	Singleton<ResourceManager>::GetInstance().PlayBaseBGM(0);

	// �v���C���[�̏�����
	for (auto& index : Singleton<GameManager>::GetInstance().mGetUsePlayer()){
		auto gearframe = Singleton<ResourceManager>::GetInstance().mGetPlayerHash(index.second);
		m_charaEntity.SetCamera(gearframe->m_pBody, &m_view);
		m_players.mSetPlayer(index.second, m_pField->mGetLane(index.second)->property._transform._translation,gearframe);
	}
	return true;
}

void SceneBattle::Finalize(){
	m_pOrderList.release();
	return;
}

bool SceneBattle::Updater(){
	m_rhythm->mAcquire();
	m_view.Controller();


	if (m_processState == eGameState::ePreCountIn){
		m_processState = eGameState::eCountIn;
		m_prevWholeBeatNo = (int)(m_rhythm->mWholeBeatTime()+0.1f);
	}
	else if (m_processState == eGameState::eCountIn){
		mCountIn();
	}
	else if (m_processState == eGameState::eUpdate){
		switch (m_battleState)
		{
		case GameManager::eBattleState::eWin:
			break;
		case GameManager::eBattleState::eLose:
			break;
		case GameManager::eBattleState::eListen:
			mOnListen();
			break;
		case GameManager::eBattleState::ePerform:
			mOnPerform();
			break;
		case GameManager::eBattleState::eBattle:
			mOnBattle();
			break;
		case GameManager::eBattleState::eCheck:
			mCheckBattle();
			break;
		case GameManager::eBattleState::eNull:
			break;
		default:
			break;
		}

	}
	else if (m_processState == eGameState::eFin){
		m_battleState = GameManager::eBattleState::eNull;
	}
	m_pOrderList->mUpdate();
	m_players.mUpdate(0, eMusical::eNull);
	return true;
}

void SceneBattle::Render(){
	m_view.Render();
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pField->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	m_players.mRender(shaderHash["texture"].get());
	return;
}

void SceneBattle::UIRender(){
	auto& shaderHash = Singleton<ResourceManager>::GetInstance().mGetShaderHash();
	m_pOrderList->mRender(shaderHash["transparent"].get(), shaderHash["color"].get());
	return;
}

bool SceneBattle::TransitionIn(){
	return kTransitionEnd;
}

bool SceneBattle::TransitionOut(){
	return kTransitionEnd;
}



// �G�̉��t
void SceneBattle::mOnListen(){
	if (m_battleState != GameManager::eBattleState::eListen)return;
	// TODO: �G�̉��t�����鏈�����X�^�b�N����鏈�����G�ɂ���Ă̓I�[�_�[���X�g�ɍ׍H������

	if (!m_InitUpdateProcess){
		
		m_pOrderList->mAddEnemyOrder(EnemyVector);
		m_pOrderList->mPlay();
		m_InitUpdateProcess = true;
	}

	//����z
	m_pField->mUpdate(m_pOrderList->mGetActionCommand());

	if (m_pOrderList->mIsEnd()){
		m_InitUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::ePerform;
	}

	return;
}

// �v���C���[�̉��t
void SceneBattle::mOnPerform(){
	if (m_battleState != GameManager::eBattleState::ePerform)return;
	// TODO: �v���C���[�̉��t���鏈��

	if (!m_InitUpdateProcess){
		m_pOrderList->mPlay();
		m_InitUpdateProcess = true;
	}
	if (m_pOrderList->mIsEnd()){
		m_InitUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::eBattle;
	}

	return;
}

// �퓬�J�n
void SceneBattle::mOnBattle(){
	if (m_battleState != GameManager::eBattleState::eBattle)return;
	// TODO: OnListen��OnPerform�̌��ʂ𔽉f���鏈��
	if (!m_InitUpdateProcess){
		m_pOrderList->mPlay();
		m_InitUpdateProcess = true;
	}
	if (m_pOrderList->mIsEnd()){
		m_InitUpdateProcess = false;
		m_processState = eGameState::ePreCountIn;
		m_battleState = GameManager::eBattleState::eListen;
	}
	return;
}

// ����p
void SceneBattle::mCheckBattle(){
	if (m_battleState != GameManager::eBattleState::eCheck)return;
	// TODO: �������菈�����ꍇ�ɂ���Ă�Wave��i�߂鏈��
}

void SceneBattle::mCountIn(){
	static int cnt = 0;
	double Padding;
	float exFrame = modf(m_rhythm->mWholeBeatTime(), &Padding);

	//����d�l�@Prefrom�̎�����
	if (m_battleState == GameManager::eBattleState::ePerform){
		if (int(m_rhythm->mWholeBeatTime() + 0.03f) != m_prevWholeBeatNo){
			cnt = 0;
			m_processState = eGameState::eUpdate;
			Debug::mPrint("Play!!");
			return;
		}
	}
	else {		//���i�͂�����
		if (int(m_rhythm->mWholeBeatTime() + 0.1f) != m_prevWholeBeatNo){
			//		std::cout << m_prevWholeBeatNo <<" "<< int(m_rhythm->mWholeBeatTime() + 0.1f) <<std::endl;
			cnt = 0;
			m_processState = eGameState::eUpdate;
			Debug::mPrint("Play!!");
			return;
		}
	}
	if (m_rhythm->mIsQuarterBeat()){
		cnt++;
//		printf("%d\n", cnt);
	}

	if (cnt >= 2){
		if (m_battleState == GameManager::eBattleState::eListen){
			Debug::mPrint("Listen");
		}
		else if (m_battleState == GameManager::eBattleState::ePerform){
			Debug::mPrint("Perform");
		}
		else if (m_battleState == GameManager::eBattleState::eBattle){
			Debug::mPrint("Battle");
		}
	}

}