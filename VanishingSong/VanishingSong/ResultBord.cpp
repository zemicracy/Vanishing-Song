#include "ResultBord.h"
#include <Rectangle2D.h>
#include <GameClock.h>
#include <GameController.h>
#include <Singleton.h>
#include <ModelUtility.h>
#include "Const.h"
#include "ResourceManager.h"
using namespace aetherClass;
using namespace aetherFunction;
namespace{
	const float kRevision = 5;
}
ResultBord::ResultBord()
{
}


ResultBord::~ResultBord()
{
	
}

void ResultBord::mInitialize(GameManager::eGameMode mode){

	// 倒した数の数字表示オブジェクト用
	for (auto& killObject : m_pKill){
		killObject = std::make_shared<Rectangle2D>();
		killObject->Initialize();
	}

	// 最大コンボ数表示用
	for (auto& comboObject : m_pKill){
		comboObject = std::make_shared<Rectangle2D>();
		comboObject->Initialize();
	}

	// 取得した音符の数表示用
	for (auto& noteObject : m_pNote){
		noteObject = std::make_unique<Rectangle2D>();
		noteObject->Initialize();
	}

	m_pMainBord = std::make_unique<Rectangle2D>();
	m_pMainBord->Initialize();

	// 次へとか終了とかのオブジェクト
	m_pClickObject= std::make_unique<Rectangle2D>();
	m_pClickObject->Initialize();
	m_pClickObject->property._transform._scale = Vector3(200,100,0);
	Vector2 scale = Vector2(m_pClickObject->property._transform._scale._x, m_pClickObject->property._transform._scale._y);
	m_pClickObject->property._transform._translation = Vector3(kWindowWidth - scale._x - kRevision, kWindowHeight - scale._y - kRevision, 0);
	m_pClickObject->property._color = Color(1, 1, 0, 1);

	// ぼかし用
	m_pBlurObject = std::make_unique<Rectangle2D>();
	m_pBlurObject->Initialize();
	m_pBlurObject->property._transform._scale = Vector3(kWindowWidth, kWindowHeight, NULL);
	m_pBlurObject->property._color = Color(0, 0, 0, 0.8);

	

	// テクスチャの登録
	m_pClickTexture.clear();
	RegisterTexture(eClickState::eNextDay, "Texture\\Exit.png");
	RegisterTexture(eClickState::eExit, "Texture\\Next.png");

	m_mode = mode;
	m_day = GameManager::eDay::eNull;
	mReset();
	
	return;
}


void ResultBord::mFinalize(){
	// 倒した数の数字表示オブジェクト用
	for (auto& killObject : m_pKill){
		if (!killObject)continue;
		killObject->Finalize();
	}

	// 最大コンボ数表示用
	for (auto& comboObject : m_pKill){
		if (!comboObject)continue;
		comboObject->Finalize();
	}

	// 取得した音符の数表示用
	for (auto& noteObject : m_pNote){
		if (!noteObject)continue;
		noteObject->Finalize();
	}

	if (m_pMainBord){
		m_pMainBord->Finalize();
	}

	if (m_pClickObject){
		m_pClickObject->Finalize();
	}
	
	if (m_pBlurObject){
		m_pBlurObject->Finalize();

	}
	
	m_pClickTexture.clear();
	
	m_mode = GameManager::eGameMode::eNull;
	m_day = GameManager::eDay::eNull;
	mReset();
	return;
}


ResultBord::eClickState ResultBord::mUpdate(Vector2 mouse, bool isClick){
	if (!m_isSetup)return eClickState::eNull;
	const float x = m_pClickObject->property._transform._translation._x;
	const float y = m_pClickObject->property._transform._translation._y;
	const float width = m_pClickObject->property._transform._scale._x + x;
	const float height = m_pClickObject->property._transform._scale._y+y;

	DirectXEntity directX;
	Vector2 screenSize = GetWindowSize(directX.GetWindowHandle(kWindowName));
	//ウィンドウサイズ調整
	mouse._x = (mouse._x / (screenSize._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screenSize._x;
	mouse._y = (mouse._y / (screenSize._y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CXDLGFRAME)))* screenSize._y;
	
	if (mouse._x > x&&mouse._x<width&&
		mouse._y>y&&mouse._y < height){
		if (isClick){
			
			mReset();
			return m_clickState;
		}
	}

	// 
	m_updateTime += GameClock::GetDeltaTime();
	return eClickState::eNull; 
}

// mUpdateを呼ぶ前にデータのセットを行う
void ResultBord::mSetupData(ResultData data,GameManager::eDay day){
	if (m_isSetup)return;
	//　データの取得
	m_data = data;
	m_day = day;

	// それぞれの数に合わせたテクスチャの適用
	mAttachTexture(m_pKill,data._killEnemy);
	mAttachTexture(m_pCombo, data._maxCombo);

	// クリックするオブジェクトのテクスチャを状況によって変える
	m_clickState = mGetClickState(m_mode, m_day);
	m_pClickObject->SetTexture(m_pClickTexture[m_clickState].get());

	m_isSetup = true;
	return;
}

void ResultBord::mAttachTexture(std::array<std::shared_ptr<aetherClass::SpriteBase>, kMaxNumber>& bord, const int count){
	
	std::string numberString;
	numberString = std::to_string(count);
	int stringSize = numberString.size();
	if (stringSize == 1){
	//	bord[0]->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture("0").get());
	//	bord[1]->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture(numberString).get());
	}
	else if (stringSize == 2){
		for (int id = 0; id < kMaxNumber; ++id){
			std::string oneString;
			oneString = numberString[id];
		//	bord[id]->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture(oneString).get());
		}
	}
	else{
		// １００以上の場合は99でカンスト
		for (auto& index : bord){
		//	index->SetTexture(Singleton<ResourceManager>::GetInstance().GetTexture("9").get());
		}
	}
	return;
}


void ResultBord::mRender(ShaderBase* defaultShader, ShaderBase* bluar ){
	// ぼかしよう
	m_pBlurObject->Render(bluar);

	m_pMainBord->Render(defaultShader);

	for (auto& killObject : m_pKill){
		killObject->Render(defaultShader);
	}

	// 最大コンボ数表示用
	for (auto& comboObject : m_pKill){
		comboObject->Render(defaultShader);
	}

	// 取得した分だけ表示
	for (int id = 0; id < m_data._getNote; ++id){
		m_pNote[id]->Render(defaultShader);
	}
	m_pClickObject->Render(defaultShader);
	return;
}


void ResultBord::mReset(){
	m_updateTime = 0.0f;
	m_isSetup = false;
	m_data.mReset();
}


ResultBord::eClickState ResultBord::mGetClickState(GameManager::eGameMode mode, GameManager::eDay day){
	if (mode == GameManager::eGameMode::ePractice){
		return eClickState::eExit;
	}

	if (day == GameManager::eDay::eLastDay || day == GameManager::eDay::eNull){
		return eClickState::eExit;
	}

	return eClickState::eNextDay;
}

void ResultBord::RegisterTexture(ResultBord::eClickState key, std::string texturePath){
	
	// 登録済みなら何もしない
	if (m_pClickTexture.find(key) != m_pClickTexture.end())return;

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->Load(texturePath);
	m_pClickTexture.insert(std::make_pair(key, texture));

	return;
}