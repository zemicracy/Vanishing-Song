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
void PlayerHPGauge::mSetProperty(SpriteBase::Property pro){
	m_property = pro;
}

bool PlayerHPGauge::mInitialize(){
	bool result = false;
	
	m_pMainSprite = std::make_shared<Rectangle2D>();
	m_pMainSprite->Initialize();

	
	m_maskTexture = std::make_shared<Texture>();
	m_maskTexture->Load("Texture\\Game\\mpGauge.png");

	m_pMainSprite->property = m_property;

	//m_pMainSprite->property._transform._rotation._z = 180;
	m_pMainSprite->SetTexture(m_maskTexture.get());


	m_pDamageSprite = std::make_shared<Rectangle2D>();
	*m_pDamageSprite.get() = *m_pMainSprite.get();
	m_pDamageSprite->property._color = Color(1, 0, 0, 1);

	m_pMaskSprite = std::make_shared<Rectangle2D>();
	*m_pMaskSprite.get() = *m_pMainSprite.get();
	m_pMaskSprite->property._color = Color(0, 0, 0, 1);


	//ìhÇËï˚ÇÃéwíË
	m_fillType._flg = 1;

	//è„â∫îΩì]
	m_fillType._direction = Vector2(1, -1);
	m_fillType._interpolation = 1;
	m_fillType._beginRadius = 0;
	m_fillType._endRadius = 360;
	m_fillType._position = Vector2(0.5, 0.5);

	
	m_maxInterPolate = 1;
	m_damageValue._interpolation = m_fillType._interpolation;
	return true;
}

void PlayerHPGauge::mUpdate(float timeScale){
	mException();


	//ê‘Ç¢ÉQÅ[ÉWäJénà íuåàÇﬂ
	if ((m_damageValue._prevHp - m_CharaStatus->_hp > 0) && m_damageValue._intervalTime <= 0 && !m_damageValue._isPlay){
		m_damageValue._morePrevHp = m_damageValue._prevHp;
		m_damageValue._intervalTime = 1;
		m_damageValue._isPlay = false;
	}
	//åàÇﬂÇΩå„Ç…èÛë‘ïœâªÇ™Ç†ÇÍÇŒéûä‘ÇêLÇŒÇ∑ -ï˚å¸ÇÃÇ›
	else if (m_damageValue._prevHp - m_CharaStatus->_hp > 0 && m_damageValue._intervalTime > 0){
		m_damageValue._intervalTime = 1;
		m_damageValue._isPlay = false;
	}
	//âΩÇ‡ñ≥ÇØÇÍÇŒéûä‘ÇçÌÇÈ
	else if (m_damageValue._intervalTime > 0){
		m_damageValue._intervalTime -= GameClock::GetDeltaTime();
		m_damageValue._interpolation = m_damageValue._morePrevHp / (float)m_CharaStatus->_maxHp * m_maxInterPolate;
		m_damageValue._isPlay = false;
	}
	//é¿çsèàóù
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
