#include "PlayerMPGauge.h"
#include"Rectangle2D.h"
#include"GameController.h"
#include<WorldReader.h>
#include"Debug.h"
#include"GameClock.h"

using namespace aetherClass;
PlayerMPGauge::PlayerMPGauge()
{
}


PlayerMPGauge::~PlayerMPGauge(){
	mFinalize();
}


void PlayerMPGauge::mFinalize(){
	if (m_pMainSprite){
		m_pMainSprite->Finalize();
		m_pMainSprite.reset();
	}
	if (m_pMaskSprite){
		m_pMaskSprite->Finalize();
		m_pMaskSprite.reset();
	}
	if (m_pIfuseSprite){
		m_pIfuseSprite->Finalize();
		m_pIfuseSprite.reset();
	}

	if (m_maskTexture){
		m_maskTexture.reset();
	}
	m_CharaStatus = nullptr;
}
bool PlayerMPGauge::mInitialize(){
	bool result = false;
	
	m_pMainSprite = std::make_shared<Rectangle2D>();
	result = m_pMainSprite->Initialize();
	if (!result)return false;

	m_maskTexture = std::make_shared<Texture>();
	result = m_maskTexture->Load("Texture\\Game\\mpGauge.png");
	if (!result)return false;

	WorldReader reader;
	reader.Load("data/gaugePosition.aether");
	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "mpGauge"){
			m_pMainSprite->property._transform = itr->_transform;
			m_pMainSprite->property._color = itr->_color;
		}
	}
	reader.UnLoad();

	//下から上にするために反転
	m_pMainSprite->property._transform._rotation._z = 180;
	m_pMainSprite->SetTexture(m_maskTexture.get());

	//コピー
	m_pIfuseSprite = std::make_shared<Rectangle2D>();
	*m_pIfuseSprite.get() = *m_pMainSprite.get();
	m_pIfuseSprite->property._color = Color(0.5, 1, 0.6, 1);
	
	m_useColor = m_pIfuseSprite->property._color;
	m_deadColor = Color(1, 0, 0, 1);

	m_pMaskSprite = std::make_shared<Rectangle2D>();
	*m_pMaskSprite.get() = *m_pMainSprite.get();
	m_pMaskSprite->property._color = Color(0, 0, 0, 1);

	//塗り方の指定
	m_direcType._direction = Vector2(0, 1);
	m_direcType._interpolation = 1;

	//割合最大値（direction塗りの場合は以下の処理が必要)
	m_maxInterPolate = m_direcType._direction.GetVectorLength()/2;

	return true;
}

void PlayerMPGauge::mUpdate(float timeScale){
	if (m_CharaStatus->_mp < m_useValue._nextMp){
		m_pIfuseSprite->property._color = m_deadColor;
	}else
		m_pIfuseSprite->property._color = m_useColor;


	//更新処理
	if (m_useValue._nextMp == 0 && m_useValue._prevNextMp != m_useValue._nextMp){
		m_useValue._morePrevMp = m_useValue._prevMp;
		m_useValue._intervalTime = 2;
	}
	//徐々に削る処理
	if (m_useValue._intervalTime > 0){
		m_useValue._morePrevMp += (m_CharaStatus->_mp - m_useValue._morePrevMp) / 10;
		m_useValue._intervalTime -= GameClock::GetDeltaTime();
		m_direcType._interpolation = (float)m_useValue._morePrevMp / (float)m_CharaStatus->_maxmp * m_maxInterPolate;
	}
	else{
		//現在のキャラのMP
		m_direcType._interpolation = (float)m_CharaStatus->_mp / (float)m_CharaStatus->_maxmp * m_maxInterPolate;
	}

	//使用後のキャラのMP
	m_useValue._interpolation = (m_CharaStatus->_mp - m_useValue._nextMp) / (float)m_CharaStatus->_maxmp * m_maxInterPolate;
	//前の状態
	m_useValue._prevMp = m_CharaStatus->_mp;
	m_useValue._prevNextMp = m_useValue._nextMp;
}

void PlayerMPGauge::mRender(std::shared_ptr<HalfFillShader> shader){
	shader->_property = m_direcType.mCastFillType();

	shader->_property._interpolation = 1;
	m_pMaskSprite->Render(shader.get());

	//現在のMPと消費予定は逆に表示する
	shader->_property._interpolation = m_direcType._interpolation;
	m_pIfuseSprite->Render(shader.get());

	shader->_property._interpolation = m_useValue._interpolation;
	m_pMainSprite->Render(shader.get());
}

void PlayerMPGauge::mSetCharaStatus(CharaStatus *status){
	m_CharaStatus = status;
}

void PlayerMPGauge::mException(){
	if (m_direcType._interpolation > m_maxInterPolate){
		m_direcType._interpolation = m_maxInterPolate;
	}
	else if (m_direcType._interpolation < 0){
		m_direcType._interpolation = 0;
	}
}


void PlayerMPGauge::mSetuseMpValue(float value){
	m_useValue._nextMp = value;
}