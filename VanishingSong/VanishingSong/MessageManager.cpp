#include "MessageManager.h"
#include "ResourceManager.h"
#include "GameController.h"
#include "GameManager.h"
#include "ResourceManager.h"
using namespace aetherClass;
namespace{
	const int kFirst = 0;
	const int kCounterNull = 0;
	const float kMessageFlameTime = 1.0f;
	const int kBossNumber = 4;
	const Vector3 kMessageFlameOffset = Vector3(0,40,0);
}

MessageManager::MessageManager(std::shared_ptr<FieldEnemyManager> enemy, aetherClass::ViewCamera* camera)
{
	m_message.mInitialize();
	m_isView = false;
	m_counter = kCounterNull;
	m_enemy = enemy;

	m_pCursor = std::make_shared<Rectangle2D>();
	m_pCursor->Initialize();
	m_pCursor->property._transform._translation._y = 630;
	m_pCursor->property._transform._scale = Vector3(120, 50, 0);
	m_pCursor->property._color = Color(0.f, 0.f, 0.f, 1.0f);
	m_pCursor->SetTexture(ResourceManager::mGetInstance().GetTexture("cursor").get());

	m_buttonTexture[eState::eNext].Load("Texture\\Message\\nextButton.png");
	m_buttonTexture[eState::eCannotSelect].Load("Texture\\Message\\nextButton.png");
	m_buttonTexture[eState::eEnd].Load("Texture\\Message\\nextButton.png");
	m_buttonTexture[eState::eSelect].Load("Texture\\Message\\yesno.png");

	m_buttonSE.first.Load("Sound\\Field\\message.wav");
	m_buttonSE.second.Load("Sound\\Field\\select.wav");
	const float volume = GameManager::mGetInstance().mGetVolume();
	m_buttonSE.first.SetValume(volume);
	m_buttonSE.second.SetValume(volume);

	m_messageFlame = std::make_shared<Rectangle3D>();
	m_messageFlame->Initialize();
	m_messageFlame->SetCamera(camera);
	m_messageFlame->property._transform._scale = Vector3(10, 6, 0);

	// カーソルの位値
	m_cursorPosition[eSelectType::eYes] = 485.f;
	m_cursorPosition[eSelectType::eNo] = 780.f;

	m_state = eState::eNull;
	m_select = true;
	m_isChangeScene = false;
	m_camera = camera;
	m_changeMessageFlame = false;
	m_messageFlameTime = NULL;
}


MessageManager::~MessageManager()
{
	if (m_messageFlame){
		m_messageFlame->Finalize();
		m_messageFlame.reset();
		m_messageFlame = nullptr;
	}

	if (m_pCursor){
		m_pCursor->Finalize();
		m_pCursor = nullptr;
	}

	m_buttonTexture.clear();
}

//
void MessageManager::mUpdate(const std::pair<int, bool> pair, const bool isPressButton, const bool isCursor, Vector3 position, Vector3 enemyPosition,const int nowStage){
	bool isEnd = false;
	const float volume = GameManager::mGetInstance().mGetVolume();
	m_buttonSE.first.SetValume(volume);
	m_buttonSE.second.SetValume(volume);
	if (!pair.second)return;
	if (pair.first == kBossNumber&&!m_enemy->mGetBossFlg())return;
	const int kEnd = m_enemy->mEnemyGet(pair.first)->mGetMessageNum();
	m_viewMessageFlame = true;
	m_messageFlame->property._transform._translation = enemyPosition + kMessageFlameOffset;

	// アイコンの設置
	m_message.mSetIcon(m_enemy->mEnemyGet(pair.first)->mGetIcon().get());

	// 話してるときは何もしない
	if (m_isView){
		m_enemy->mEnemyGet(pair.first)->mIsTalking(true);
		m_viewMessageFlame = false;
		m_message.mUpdate((m_state == eState::eSelect));
	}
	
	if (m_viewMessageFlame){
		m_messageFlameTime += GameClock::GetDeltaTime();
		if (m_messageFlameTime > kMessageFlameTime){
			m_changeMessageFlame = !m_changeMessageFlame;
			m_messageFlameTime = NULL;
		}
	}

	if (m_changeMessageFlame){
		m_messageFlame->SetTexture(ResourceManager::mGetInstance().GetTexture("comment").get());
	}
	else{
		m_messageFlame->SetTexture(ResourceManager::mGetInstance().GetTexture("comment2").get());
	}

	if (isPressButton){
		m_buttonSE.first.Stop();
		m_buttonSE.first.PlayToOneTime();
		// シーンの遷移？
		if (m_state == eState::eSelect && m_selectType == eSelectType::eYes){
			m_isChangeScene = true;
			return;
		}

		if (m_counter >= kEnd){
			isEnd = true;
		}
		else{
			// メッセージの切り替え
			if (m_state == eState::eCannotSelect){
				m_messageBuffer = std::make_shared<Texture>();
				m_messageBuffer->Load(m_enemy->mEnemyGet(pair.first)->mGetCannotMessga());
				mChangeMessage(m_messageBuffer.get());
			}
			else{
				m_messageBuffer = std::make_shared<Texture>();
				m_messageBuffer->Load(m_enemy->mEnemyGet(pair.first)->mGetMessage(m_counter));
				mChangeMessage(m_messageBuffer.get());
			}
		}

		
		// 最初だけプレイヤーの方を向かせるようにする
		if (m_counter == kFirst){
			m_enemy->mEnemyGet(pair.first)->mFaceToPlayer(position);
			m_isChangeScene = false;
			m_isView = true;
			m_state = eState::eNext;
			m_selectType = eSelectType::eNull;
		}

		// 状態の切り替え
		if (m_counter == kEnd){
			m_state = eState::eEnd;
		}
		else if (m_counter == kEnd - 2){
			// そのステージを受ける資格があるかの判断
			if (pair.first <= nowStage){
				m_state = eState::eSelect;
			}
			else{
				m_state = eState::eCannotSelect;
			}
		}
		else{
			m_state = eState::eNext;
		}

		m_message.mSetButton(&m_buttonTexture[m_state]);
		
		m_counter += 1;

	}
	
	
	// カーソルの更新
	mCursorUpdate(isCursor);

	if (isEnd){
		m_isView = false;
		m_select = true;
		m_counter = kCounterNull;
		m_isChangeScene = false;
		m_state = eState::eNext;
		m_selectType = eSelectType::eNull;
		m_enemy->mEnemyGet(pair.first)->mIsTalking(false);

		if (GameManager::mGetInstance().mGetCanStage()-1<pair.first){
			m_enemy->mEnemyGet(pair.first)->mResetTransform();
		}
	}
}

//
void MessageManager::mChangeMessage(aetherClass::Texture* tex){
	m_message.mSetText(tex);
}

//
void MessageManager::m2DRender(aetherClass::ShaderBase* trans, aetherClass::ShaderBase* col){
	if (!m_isView)return;
	m_message.mRender(trans);
	if (m_state == eState::eSelect){
		m_pCursor->Render(trans);
	}
}

//
void MessageManager::m3DRender(aetherClass::ShaderBase* tex, aetherClass::ShaderBase* col){
	if (m_isView)return;
	if (m_viewMessageFlame){
		m_messageFlame->property._transform._rotation = m_camera->property._rotation;
		m_messageFlame->Render(tex);
		m_viewMessageFlame = false;
	}
}
//
bool MessageManager::mIsView(){
	return m_isView;
}

//
void MessageManager::mCursorUpdate(const bool flg){
	if (m_state != eState::eSelect)return;
	if (flg){
		m_buttonSE.second.Stop();
		m_buttonSE.second.PlayToOneTime();
		m_select = !m_select;
	}

	if (m_select){
		m_selectType = eSelectType::eYes;
	}
	else{
		m_selectType = eSelectType::eNo;
	}

	m_pCursor->property._transform._translation._x = m_cursorPosition[m_selectType];
}

bool MessageManager::mGetIsChangeScene()const{
	return m_isChangeScene;
}

void MessageManager::mSetIcon(Texture* tex){
	m_message.mSetIcon(tex);
}