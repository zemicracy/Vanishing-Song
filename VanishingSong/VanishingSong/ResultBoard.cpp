#include"ResultBoard.h"
#include"WorldReader.h"
#include"Rectangle2D.h"
#include"GameClock.h"
#include"ResourceManager.h"

using namespace aetherClass;
ResultBoard::ResultBoard(){

}

ResultBoard::~ResultBoard(){
	mFinalize();
}

void ResultBoard::mFinalize(){
	m_halfFill->Finalize();
	m_pGauge.reset();
	
	for (auto &itr : m_pGeneral){
		itr.second->Finalize();
	}
	m_pGeneral.clear();
	m_pNumSprite->Finalize();

	for (auto& itr : m_numberList){
		itr.second.reset();
	}
	for (auto& itr : m_TextureList){
		itr.second.reset();
	}
	m_numberList.clear();
	m_TextureList.clear();
}


template<class T>
std::shared_ptr<T> gInitializer(aetherClass::Transform transform, aetherClass::Color color){
	auto obj = std::make_shared<T>();
	obj->Initialize();
	obj->property._transform = transform;
	obj->property._color = color;
	return obj;
}

std::shared_ptr<aetherClass::Texture> gLoadTexture(std::string path){
	auto obj = std::make_shared<Texture>();
	obj->Load(path);
	return obj;
}

void ResultBoard::mInitialize(){
	WorldReader reader;
	reader.Load("data\\Result.aether");


	Color BLACK(0, 0, 0, 0);
	for (auto& itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "correctGauge"){
			m_pGauge = std::make_shared<ClearGauge>();
			m_pGauge->mSetTransform(itr->_transform);
			m_pGauge->mInitialize();
			m_pGauge->mSetRate(0.5);
		}
		else if (itr->_name == "correctRate"){
			m_rankRatePosision = itr->_transform._translation;
		}
		else if (itr->_name == "missValue"){
			m_missCountPosision = itr->_transform._translation;
		}
		else if (itr->_name == "backboard"){
			m_pGeneral.insert(std::make_pair(itr->_name, gInitializer<Rectangle2D>(itr->_transform, itr->_color)));
		}
		else if (itr->_name != ""){
			m_pGeneral.insert(std::make_pair(itr->_name, gInitializer<Rectangle2D>(itr->_transform, BLACK)));
		}
	}


	m_noteScaleOrigin = m_pGeneral["noteImage"]->property._transform._scale._x;
	m_noteTransOrigin = m_pGeneral["noteImage"]->property._transform._translation + m_noteScaleOrigin / 2;

	m_pGeneral["backboard"]->property._color._alpha = 0;

	m_pNumSprite = std::make_shared<Rectangle2D>();
	m_pNumSprite->Initialize();
	m_pNumSprite->property._transform._scale = Vector3(25, 40, 1);
	m_pNumSprite->property._color = Color(0, 0, 0, 1);

	std::string path = "Texture\\Result\\";
	m_TextureList["rankText"] = gLoadTexture(path + "rank.png");
	m_pGeneral["rankText"]->SetTexture(m_TextureList["rankText"].get());

	m_TextureList["miss"] = gLoadTexture(path + "Miss.png");
	m_pGeneral["missText"]->SetTexture(m_TextureList["miss"].get());

	m_TextureList["correct"] = gLoadTexture(path + "correct_rate.png");
	m_pGeneral["correctRateText"]->SetTexture(m_TextureList["correct"].get());

	m_TextureList["blank"] = gLoadTexture(path + "blank.png");
	m_pGeneral["backboard"]->SetTexture(m_TextureList["blank"].get());

	m_TextureList["return"] = gLoadTexture("Texture\\ActionCommand\\Red.png");
	m_pGeneral["return"]->SetTexture(m_TextureList["return"].get());


	ShaderDesc desc;
	desc._pixel._srcFile = L"Shader/HalfFiller.hlsl";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._vertex._entryName = "vs_main";

	m_halfFill = std::make_shared<HalfFillShader>();
	m_halfFill->Initialize(desc, eVertex | ePixel);

	for (int i = 0; i < 10; ++i){
		m_numberList[std::to_string(i)] = gLoadTexture("Texture\\Number\\" + std::to_string(i) + ".png");
	}
	m_numberList["."] = gLoadTexture("Texture\\Number\\dot.png");
	m_numberList["%"] = gLoadTexture("Texture\\Number\\%.png");

	m_timer = 0;

	reader.UnLoad();
	m_isEnd = false;
	m_state = eNone;
	m_isNoteGet = false;
}

void ResultBoard::mSetResultData(ResultData result,GameManager::eBattleState state,UINT stageID){
	m_resultData = result;

	const float S_Border = 0.80;
	const float A_Border = 0.65;
	const float B_underBorder = 0.50;
	const int noteBorder = 80;


	float rate = (float)m_resultData._missCount / m_resultData._maxCount;
	rate = 1 - rate;
	m_pGauge->mSetRate(0);
	m_MaxRate = rate;
	if (state == GameManager::eBattleState::eWin){
		m_TextureList["issue"] = gLoadTexture("Texture\\Result\\Win.png");
		int stage = GameManager::mGetInstance().mGetCanStage();
		stage = stage < stageID ? stageID : stage;
		GameManager::mGetInstance().mGetCanStage(stage);
	}
	else{
		m_TextureList["issue"] = gLoadTexture("Texture\\Result\\Lose.png");
	}
	m_pGeneral["battleResult"]->SetTexture(m_TextureList["issue"].get());

	std::string path = "Texture\\Result\\rank\\";
	
	m_TextureList["note"] = gLoadTexture("Texture\\OrderList\\note.png");
	if (rate >= S_Border&& state == GameManager::eBattleState::eWin){
		m_TextureList["rank"] = gLoadTexture(path + "S.png");
		m_isNoteGet = true;
	}
	else{
		if (rate >= A_Border && state == GameManager::eBattleState::eWin){
			m_TextureList["rank"] = gLoadTexture(path + "A.png");
		}
		else if (rate >= B_underBorder && state == GameManager::eBattleState::eWin){
			m_TextureList["rank"] = gLoadTexture(path + "B.png");
		}
		else if (rate >= B_underBorder){
			m_TextureList["rank"] = gLoadTexture(path + "C.png");
		}
		else {
			m_TextureList["rank"] = gLoadTexture(path + "D.png");
		}
	}
	m_pGeneral["rankImage"]->SetTexture(m_TextureList["rank"].get());
	m_pGeneral["noteImage"]->SetTexture(m_TextureList["note"].get());

	float i = 0;
	rate = modf(rate*100, &i);
	int dotrate = rate * 10;
	int integer = i;

	m_rateString = std::to_string(integer) + "." + std::to_string(dotrate)+"%";
//	Debug::mPrint(std::to_string(i) + "  " + std::to_string(dotrate) + "  " + std::to_string(rate));
	m_state = eInit;
	m_isEnd = false;

	if (state == GameManager::eBattleState::eLose){
		return;
	}


	auto& itr = ResourceManager::mGetInstance().mGetBGMPath();
	//ステージ曲追加分
	if (stageID == 0){
		if (itr.find(eMusical::eBlue) == itr.end() && itr.find(eMusical::eAdlib) == itr.end()){
			ResourceManager::mGetInstance().mSetBGMPath(eMusical::eBlue) = "Sound\\BGM\\field1.wav";
			GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eTutorialEnd);
		}
		m_TextureList["note"]->Load("Texture\\OrderList\\note_a.png");
		m_pGeneral["noteImage"]->SetTexture(m_TextureList["note"].get());
		m_isNoteGet = true;
	}
	else if (stageID == 1){
		GameManager::mGetInstance().mPushUsePlayer(eMusical::eGreen);
		if (integer < noteBorder)return;		//レート0.90以上で音符の取得

		m_pGeneral["noteImage"]->property._color = Color(0, 0, 1, 0);
		if (itr.find(eMusical::eAdlib) == itr.end())
		ResourceManager::mGetInstance().mSetBGMPath(eMusical::eBlue) = "Sound\\BGM\\field2_1.wav";
		GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eSecoundStage);
	}
	else if (stageID == 2){
		GameManager::mGetInstance().mPushUsePlayer(eMusical::eRed);
		if (integer < noteBorder)return;		//レート0.90以上で音符の取得

		m_pGeneral["noteImage"]->property._color = Color(0, 1, 0, 0);
		if (itr.find(eMusical::eAdlib) == itr.end())
		ResourceManager::mGetInstance().mSetBGMPath(eMusical::eGreen) = "Sound\\BGM\\field2.wav";
		GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eThirdStage);
	}
	else if (stageID == 3){
		GameManager::mGetInstance().mPushUsePlayer(eMusical::eYellow);
		if (integer < noteBorder)return;		//レート0.90以上で音符の取得
		
		m_pGeneral["noteImage"]->property._color = Color(1, 0, 0, 0);
		if (itr.find(eMusical::eAdlib) == itr.end())
		ResourceManager::mGetInstance().mSetBGMPath(eMusical::eRed) = "Sound\\BGM\\field3.wav";
		GameManager::mGetInstance().mFieldState(GameManager::eFieldState::eForthStage);
	}
	else if (stageID == 4){
		if (GameManager::mGetInstance().mBossState() != GameManager::eBossState::eWin){
			GameManager::mGetInstance().mBossState(GameManager::eBossState::eVisible);
		}
		if (integer < noteBorder)return;
		
		m_pGeneral["noteImage"]->property._color = Color(1, 1, 0, 0);
		if (itr.find(eMusical::eAdlib) == itr.end())
		ResourceManager::mGetInstance().mSetBGMPath(eMusical::eYellow) = "Sound\\BGM\\field4.wav";
	}
	else if(stageID == 5){
		if (integer < noteBorder)return;
		int cnt = 0;
		for (auto &path : itr){
			cnt++;
		}		//ボスは音符全部で
		if (cnt >= 4){
			m_TextureList["note"]->Load("Texture\\OrderList\\note_a.png");
			itr.clear();
			ResourceManager::mGetInstance().mSetBGMPath(eMusical::eAdlib) = "Sound\\BGM\\field5.wav";
		}
		GameManager::mGetInstance().mBossState(GameManager::eBossState::eWin);
	}
}
bool ResultBoard::mIsEnd(){
	return m_isEnd;
}


void ResultBoard::mUpdate(float timeScale){
	switch (m_state){
	case ResultBoard::eNone:
		return;
	case ResultBoard::eInit:
		if (m_pGeneral["backboard"]->property._color._alpha > 0.85){
			m_pGeneral["backboard"]->property._color._alpha = 0.85;
			m_state++;
			m_timer = 0;
		}
		else{
			m_pGeneral["backboard"]->property._color._alpha += 0.1 * timeScale;
		}
		break;
	case ResultBoard::eResultTitle:
	{
		if (m_timer > 1){
			m_pGeneral["battleResult"]->property._color._alpha = 1;
			m_timer = 0;
			m_state++;
		}
		else{
			m_timer += GameClock::GetDeltaTime() * timeScale;
		}
	}
	break;
	case ResultBoard::eMissCnt:
	{
		if (m_timer > 1){
			m_pGeneral["missText"]->property._color._alpha = 1;
			m_timer = 0;
			m_state++;

			mReloadSound("Sound\\Result\\gauge.wav", -2000);
		}
		else{
			m_timer += GameClock::GetDeltaTime() * timeScale;
		}
	}
	break;
	case ResultBoard::eClearGauge:
	{
		m_pSoundDevice->PlayToLoop();
			m_pGeneral["correctRateText"]->property._color._alpha = 1;
			if (m_MaxRate <= m_timer){
				m_timer = 0;
				m_state++;
				m_pGauge->mSetRate(m_MaxRate);

				mReloadSound("Sound\\Result\\noteGet.wav", -2000);
			}
			else{
				m_timer += 0.01 * timeScale;
				m_pGauge->mSetRate(m_timer);
			}
	}
	break;
	case ResultBoard::eRank:
	{
		m_pGeneral["rankText"]->property._color._alpha = 1;
		m_pGeneral["rankFrame"]->property._color._alpha = 1;
		m_pGeneral["rankImage"]->property._color._alpha = 1;
		if (m_timer > 1){
			m_timer = 1000;
			m_acceleration = 1.0;
			m_state++;
		}
		else{
			m_timer += GameClock::GetDeltaTime() * timeScale;
		}
	}
	break;
	case ResultBoard::eNote:
	{
		if (m_isNoteGet){
			m_pSoundDevice->PlayToOneTime();
			m_pGeneral["noteImage"]->property._transform._scale = m_timer + m_noteScaleOrigin; //(m_noteScaleOrigin * 15);
			m_pGeneral["noteImage"]->property._color._alpha = 1;
			if (m_timer < 0){
				m_pGeneral["noteImage"]->property._transform._scale = m_noteScaleOrigin;
				m_pGeneral["noteImage"]->property._transform._translation = m_noteTransOrigin - (m_noteScaleOrigin / 2);
				m_pGeneral["noteImage"]->property._transform._translation._z = 0;
				m_state++;
				m_timer = 0;
			}
			else{
				auto size = (m_pGeneral["noteImage"]->property._transform._scale);
				m_pGeneral["noteImage"]->property._transform._translation = m_noteTransOrigin - (size / 2);
				m_pGeneral["noteImage"]->property._transform._translation._z = 0;
				m_timer -= 10 * m_acceleration * timeScale;
				m_acceleration += 0.2;
			}
		}
		else{
			if (m_pGeneral["noteImage"]->property._color._alpha > 1){
				m_pGeneral["noteImage"]->property._color._alpha = 1;
				m_state++;
			}else
				m_pGeneral["noteImage"]->property._color._alpha += 0.05 * timeScale;
		}
	}
	break;
	case ResultBoard::eEnd:
	{
		if (m_timer > 0.5){
			m_pGeneral["return"]->property._color._alpha = 1 - m_pGeneral["return"]->property._color._alpha;
			m_timer = 0;
			m_isEnd = true;
		}else
			m_timer += GameClock::GetDeltaTime();

	}
		break;
	default:
		break;
	}

}

void ResultBoard::mRender(aetherClass::ShaderBase* shader, aetherClass::ShaderBase* debug){

	for (auto& itr : m_pGeneral){
			itr.second->Render(shader);
	}

	if (m_state > eResultState::eMissCnt){
		//missCount
		m_pNumSprite->property._transform._translation = m_missCountPosision;
		for (int i = 0; i < std::to_string(m_resultData._missCount).length(); ++i){
			char c = std::to_string(m_resultData._missCount).at(i);
			std::string st;
			st.push_back(c);

			m_pNumSprite->property._transform._translation._x += m_pNumSprite->property._transform._scale._x;
			m_pNumSprite->SetTexture(m_numberList[st].get());
			m_pNumSprite->Render(shader);
		}
	}
	if (m_state >= eResultState::eClearGauge){
		//ClearRate
		m_pGauge->mRender(m_halfFill);
		m_pNumSprite->property._transform._translation = m_rankRatePosision;
		for (int i = 0; i < m_rateString.length(); ++i){
			char c = m_rateString.at(i);
			std::string st;
			st.push_back(c);

			m_pNumSprite->property._transform._translation._x += m_pNumSprite->property._transform._scale._x;
			m_pNumSprite->SetTexture(m_numberList[st].get());
			m_pNumSprite->Render(shader);
		}
	}

}

void ResultBoard::mReloadSound(std::string file,int volume){
	if (m_pSoundDevice){
		m_pSoundDevice->Stop();
		m_pSoundDevice.reset();
	}
	m_pSoundDevice = std::make_shared<GameSound>();
	m_pSoundDevice->Load(file.c_str());
	m_pSoundDevice->SetValume(volume);

}
