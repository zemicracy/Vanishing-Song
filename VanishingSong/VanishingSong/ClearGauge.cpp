#include "ClearGauge.h"
#include"Rectangle2D.h"
#include"GameController.h"
#include<WorldReader.h>
#include"Debug.h"
#include"GameClock.h"

using namespace aetherClass;
ClearGauge::ClearGauge()
{
}


ClearGauge::~ClearGauge(){
	mFinalize();
}


void ClearGauge::mFinalize(){
	if (m_pMainSprite){
		m_pMainSprite->Finalize();
		m_pMainSprite.reset();
	}
	if (m_pMaskSprite){
		m_pMaskSprite->Finalize();
		m_pMaskSprite.reset();
	}

	if (m_maskTexture){
		m_maskTexture.reset();
	}
	if (m_mainTexture){
		m_mainTexture.reset();
	}
}
bool ClearGauge::mInitialize(){
	bool result = false;
	
	m_pMainSprite = std::make_shared<Rectangle2D>();
	result = m_pMainSprite->Initialize();
	if (!result)return false;

	m_maskTexture = std::make_shared<Texture>();
	result = m_maskTexture->Load("Texture\\Result\\blank.png");
	if (!result)return false;

	m_mainTexture = std::make_shared<Texture>();
	m_mainTexture->Load("Texture\\Result\\clearGauge.png");

	m_pMainSprite->property._transform = m_trans;
	m_pMainSprite->SetTexture(m_mainTexture.get());
	m_pMainSprite->property._color = Color(0, 0, 0, 1);

	m_pMaskSprite = std::make_shared<Rectangle2D>();
	*m_pMaskSprite.get() = *m_pMainSprite.get();
	m_pMaskSprite->SetTexture(m_maskTexture.get());
	m_pMaskSprite->property._color = Color(0, 0, 0, 1);


	//塗り方の指定
	m_direcType._direction = Vector2(1, 0);
	m_direcType._interpolation = 1;

	//割合最大値（direction塗りの場合は以下の処理が必要)
	m_maxInterPolate = m_direcType._direction.GetVectorLength()/2;

	return true;
}

void ClearGauge::mUpdate(float timeScale){
}

void ClearGauge::mRender(std::shared_ptr<HalfFillShader> shader){
	shader->_property = m_direcType.mCastFillType();

	shader->_property._interpolation = 1;
	m_pMaskSprite->Render(shader.get());

	shader->_property._interpolation = m_useValue._interpolation;
	m_pMainSprite->Render(shader.get());
}

void ClearGauge::mSetTransform(aetherClass::Transform trans){
	m_trans = trans;
}

void ClearGauge::mSetRate(float i){
	m_useValue._interpolation = i * m_maxInterPolate;
}