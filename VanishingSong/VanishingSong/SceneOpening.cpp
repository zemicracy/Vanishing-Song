#include "SceneOpening.h"
#include<iostream>
#include<PixelShader.h>
#include<Rectangle2D.h>
#include<GameController.h>

using namespace aetherClass;

SceneOpening::SceneOpening():
GameScene("SceneOpening",GetManager())
{
}


SceneOpening::~SceneOpening()
{
}

bool SceneOpening::Initialize(){

	ShaderDesc desc;
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader\\VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader\\ColorTexture.hlsl";

	m_pShaderBase = std::make_unique<PixelShader>();
	m_pShaderBase->Initialize(desc, ShaderType::eVertex | ShaderType::ePixel);

	m_pSpriteBase = std::make_unique<Rectangle2D>();
	m_pSpriteBase->Initialize();
	m_pSpriteBase->property._transform._translation = Vector3(0, 0, 0);
	m_pSpriteBase->property._transform._scale._x = 1280;
	m_pSpriteBase->property._transform._scale._y = 720;
	m_pSpriteBase->property._color = Color(0, 0, 0, 1);

	//���ԃJ�E���g�p�ϐ�
	m_clockCount = 0;

	//�z��ԍ��̐؂芷���p�ϐ�
	m_array = 0;

	//�e�N�X�`���\���J��Ԃ��p�ϐ�
	m_imageCount = 0;

	//�e�N�X�`����ǂݍ���
	for (int i = 0; i<17; i++){
		m_pTexture[i] = std::make_unique<Texture>();
		m_pTexture[i]->Load("Texture/Opening" + std::to_string(i + 1) + ".png");
	}

	//1���ڂ͂����\��
	m_pSpriteBase->SetTexture(m_pTexture[m_array].get());

	return true;
}

void SceneOpening::Finalize(){
	m_pSpriteBase->Finalize();

	return;
}

bool SceneOpening::Updater(){
	//m_clockCount�Ƀf���^�^�C���𑫂��Ă���
	m_clockCount = m_clockCount + GameClock::GetDeltaTime();

	//1�A2�J��Ԃ��\��
	if (m_imageCount < 2){
		if (m_clockCount > 1){
			m_clockCount = 0;
			if (m_array < 2){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
			}
		}
		//m_array��2�ȏ�Ȃ�
		if (m_array>1){
			m_array = 0;
			m_imageCount++;
		}
	}
	if (m_imageCount == 2){
		m_array = 2;
	}

	//3�`17�\��
	if (m_imageCount > 1){
		if (m_clockCount > 2){
			m_clockCount = 0;
			if (m_array < 17){
				m_pSpriteBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
				m_imageCount++;
			}
		}
	}

	return true;
}

void SceneOpening::Render(){
	m_pSpriteBase->Render(m_pShaderBase.get());
	return;
}

void SceneOpening::UIRender(){
	return;
}

bool SceneOpening::TransitionIn(){
	return kTransitionEnd;
}

bool SceneOpening::TransitionOut(){
	return kTransitionEnd;
}