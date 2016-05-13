#include "PlayerHPGauge.h"
#include<Rectangle2D.h>
#include<GameController.h>
#include<WorldReader.h>
#include<GameClock.h>

using namespace aetherClass;
PlayerHPGauge::PlayerHPGauge()
{
}


PlayerHPGauge::~PlayerHPGauge()
{
	mFinalize();
}


void PlayerHPGauge::mFinalize(){
	if (m_pMainSprite){
		m_pMainSprite->Finalize();
		m_pMainSprite.reset();
	}
	if (m_pMaskSprite){
		m_pMaskSprite->Finalize();
		m_pMaskSprite.reset();
	}
	if (m_pDamageSprite){
		m_pDamageSprite->Finalize();
		m_pDamageSprite.reset();
	}
	if (m_maskTexture){
		m_maskTexture.reset();
	}
	m_CharaStatus = nullptr;
}
bool PlayerHPGauge::mInitialize(){
	bool result = false;
	
	m_pMainSprite = std::make_shared<Rectangle2D>();
	result = m_pMainSprite->Initialize();
	if (!result)return false;

	m_maskTexture = std::make_shared<Texture>();
	result = m_maskTexture->Load("Texture\\Game\\hpGauge.png");
	if (!result)return false;

	WorldReader reader;
	reader.Load("data/gaugePosition.aether");
	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "hpGauge"){
			m_pMainSprite->property._transform = itr->_transform;
			m_pMainSprite->property._color = itr->_color;
		}
	}

	reader.UnLoad();
	//m_pMainSprite->property._transform._rotation._z = 180;
	m_pMainSprite->SetTexture(m_maskTexture.get());
	m_pMainSprite->property._color = Color(0, 1, 0, 1);

	m_pDamageSprite = std::make_shared<Rectangle2D>();
	*m_pDamageSprite.get() = *m_pMainSprite.get();
	m_pDamageSprite->property._color = Color(1, 0, 0, 1);

	m_pMaskSprite = std::make_shared<Rectangle2D>();
	*m_pMaskSprite.get() = *m_pMainSprite.get();
	m_pMaskSprite->property._color = Color(0, 0, 0, 1);


	//塗り方の指定
	m_fillType._flg = 1;
	//上下反転
	m_fillType._direction = Vector2(1, -1);
	m_fillType._interpolation = 1;
	m_fillType._beginRadius = 80;
	m_fillType._endRadius = 272;
	m_fillType._position = Vector2(0.5, 0.5);

	
	m_maxInterPolate = 1;
	m_damageValue._interpolation = m_fillType._interpolation;
	return true;
}

void PlayerHPGauge::mUpdate(float timeScale){
	mException();


	//赤いゲージ開始位置決め
	if ((m_damageValue._prevHp - m_CharaStatus->_hp > 0) && m_damageValue._intervalTime <= 0 && !m_damageValue._isPlay){
		m_damageValue._morePrevHp = m_damageValue._prevHp;
		m_damageValue._intervalTime = 1;
		m_damageValue._isPlay = false;
	}
	//決めた後に状態変化があれば時間を伸ばす -方向のみ
	else if (m_damageValue._prevHp - m_CharaStatus->_hp > 0 && m_damageValue._intervalTime > 0){
		m_damageValue._intervalTime = 1;
		m_damageValue._isPlay = false;
	}
	//何も無ければ時間を削る
	else if (m_damageValue._intervalTime > 0){
		m_damageValue._intervalTime -= GameClock::GetDeltaTime();
		m_damageValue._interpolation = m_damageValue._morePrevHp / (float)m_CharaStatus->_maxHp * m_maxInterPolate;
		m_damageValue._isPlay = false;
	}
	//実行処理
	if (m_damageValue._intervalTime <= 0 && m_damageValue._morePrevHp != m_damageValue._prevHp){
		m_damageValue._isPlay = true;
		m_damageValue._morePrevHp -= (m_damageValue._morePrevHp - m_CharaStatus->_hp) / 10;
		m_damageValue._interpolation = m_damageValue._morePrevHp / (float)m_CharaStatus->_maxHp * m_maxInterPolate;
	}

	m_fillType._interpolation = m_CharaStatus->_hp / (float)m_CharaStatus->_maxHp * m_maxInterPolate;
	
	m_damageValue._prevHp = m_CharaStatus->_hp;
}

void PlayerHPGauge::mRender(std::shared_ptr<HalfFillShader> shader){
	shader->_property = m_fillType;

	shader->_property._interpolation = 1;
	m_pMaskSprite->Render(shader.get());

	shader->_property._interpolation = m_damageValue._interpolation;
	m_pDamageSprite->Render(shader.get());

	shader->_property._interpolation = m_fillType._interpolation;
	m_pMainSprite->Render(shader.get());


}

void PlayerHPGauge::mSetCharaStatus(CharaStatus *status){
	m_CharaStatus = status;
}

void PlayerHPGauge::mException(){
	if (m_fillType._interpolation > m_maxInterPolate){
		m_fillType._interpolation = m_maxInterPolate;
	}
	else if (m_fillType._interpolation < 0){
		m_fillType._interpolation = 0;
	}
}
