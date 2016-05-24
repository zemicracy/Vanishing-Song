#include "MessageManager.h"
#include "GameController.h"
namespace{
	const int kFirst = 0;
	const int kCounterNull = -1;
}
using namespace aetherClass;
MessageManager::MessageManager(std::shared_ptr<FieldEnemyManager>& enemy)
{
	m_message.mInitialize();
	m_isView = false;
	m_counter = kCounterNull;
	m_enemy = enemy;
	m_pCursor = std::make_shared<Rectangle2D>();
	m_pCursor->Initialize();
	m_pCursor->property._transform._translation._y = 630;
	m_pCursor->property._transform._scale = 50;

	m_texts = std::make_shared<aetherClass::Texture>();
	m_texts->Load("Texture\\Message\\tmplate.png");

	m_buttonTexture[eState::eNext].Load("Texture\\Message\\nextButton.png");
	m_buttonTexture[eState::eEnd].Load("Texture\\Message\\nextButton.png");
	m_buttonTexture[eState::eSelect].Load("Texture\\Message\\yesno.png");

	// カーソルの位値
	m_cursorPosition[eSelectType::eYes] = 360.f;
	m_cursorPosition[eSelectType::eNo] = 670.f;

	m_state = eState::eNull;
	m_select = true;
	m_isChangeScene = false;
}


MessageManager::~MessageManager()
{
	m_message.mFinalize();
	if (m_pCursor){
		m_pCursor->Finalize();
		m_pCursor = nullptr;
	}
}

//
void MessageManager::mUpdate(const std::pair<int, bool> pair, const bool isPressButton,const bool isCursor, aetherClass::Vector3 position){
	bool isEnd = false;
	if (pair.second)return;
	const int kEnd = 2;
	if (isPressButton){
		// シーンの遷移？
		if (m_state == eState::eSelect && m_selectType == eSelectType::eYes){
			m_isChangeScene = true;
			return;
		}
		else if (m_state == eState::eSelect && m_selectType == eSelectType::eNo){
			m_counter = kEnd-1;
		}

		mChangeMessage(m_texts.get());
		
		m_counter += 1;

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
		else if(m_counter == kEnd-1){
			m_state = eState::eSelect;
		}
		else{
			m_state = eState::eNext;
		}

		m_message.mSetButton(&m_buttonTexture[m_state]);

		if (m_counter > kEnd){
			isEnd = true;
		}

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
	}
}

//
void MessageManager::mChangeMessage(aetherClass::Texture* tex){
	m_message.mSetText(tex);
}

//
void MessageManager::mRender(aetherClass::ShaderBase* shader, aetherClass::ShaderBase* color){
	if (!m_isView)return;
	m_message.mRender(shader);
	if (m_state == eState::eSelect){
		m_pCursor->Render(color);
	}
}

//
bool MessageManager::mIsView(){
	return m_isView;
}

void MessageManager::mCursorUpdate(const bool flg){
	if (m_state != eState::eSelect)return;
	if (flg){
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