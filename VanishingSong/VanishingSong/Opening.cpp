#include "Opening.h"
#include<iostream>
#include<PixelShader.h>
//#include<Rectangle2D.h>
#include<Rectangle3D.h>
#include<GameController.h>
//#include<GameClock.h>

using namespace aetherClass;

Opening::Opening():
GameScene("Opening", GetManager())
{
}


Opening::~Opening()
{
}

bool Opening::Initialize(){
	//�J����
	m_pOpeningView = std::make_unique<ViewCamera>();
	m_pOpeningView->property._translation = Vector3(0, 0, -2);

	ShaderDesc desc;
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader\\VertexShaderBase.hlsl";
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader\\ColorTexture.hlsl";

	m_pShaderBase = std::make_unique<PixelShader>();
	m_pShaderBase->Initialize(desc, ShaderType::eVertex | ShaderType::ePixel);
	
	m_pModelBase = std::make_unique<Rectangle3D>();
	m_pModelBase->Initialize();
	m_pModelBase->SetCamera(m_pOpeningView.get());
	m_pModelBase->property._transform._translation = Vector3(0, 0, 0);
	m_pModelBase->property._color = Color(0, 0, 0, 1);

	//���ԃJ�E���g�p�ϐ�
	m_clockCount = 0;

	//�z��ԍ��̐؂芷���p�ϐ�
	m_array = 0;

	m_imageCount = 0;

	//�e�N�X�`����ǂݍ���
	for (int i = 0;i<8;i++){
		m_pTexture[i] = std::make_unique<Texture>();
		m_pTexture[i]->Load("Texture/Opening"+std::to_string(i+1)+".png");
	}

	//1���ڂ�5�b�҂����ɕ\��
	m_pModelBase->SetTexture(m_pTexture[m_array].get());

	return true;
}

void Opening::Finalize(){
	m_pModelBase->Finalize();
	//m_pSpriteBase->Finalize();

	return;
}
bool Opening::Updater(){
	//m_clockCount�Ƀf���^�^�C���𑫂��Ă���
	m_clockCount = m_clockCount + GameClock::GetDeltaTime();

	//�P�A2�J��Ԃ��\��
	if (m_imageCount < 2){
		if (m_clockCount > 1){
			m_clockCount = 0;
			if (m_array < 2){
				m_pModelBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
			}
		}
		//m_array��2�ȏ�Ȃ�
		if (m_array > 1){
			m_array = 0;
			m_imageCount++;
		}
	}
	if (m_imageCount == 2){
		m_array = 2;
	}

	//3�A4�A5�\��
	if (m_imageCount > 1){
		if (m_clockCount > 2){
			m_clockCount = 0;
			if (m_array < 5){
				m_pModelBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
				m_imageCount++;
			}
		}
	}

	//6�A7�J��Ԃ��\��
	if (m_imageCount>4){
		if (m_clockCount > 1){
			m_clockCount = 0;
			if (m_array < 7){
				m_pModelBase->SetTexture(m_pTexture[m_array].get());
				m_array++;
			}
		}
		//m_array��7�ȏ� m_imageCount��7�ȉ�
		if (m_array > 6 && m_imageCount < 7){
			m_array = 5;
			m_imageCount++;
		}
	}
	if (m_imageCount == 7){
		m_array = 7;
	}

	return true;
}
void Opening::Render(){
	m_pOpeningView->Render();
	m_pModelBase->Render(m_pShaderBase.get());
	//m_pSpriteBase->Render(m_pShaderBase.get());
	return;
}
void Opening::UIRender(){
	return;
}

bool Opening::TransitionIn(){
	return kTransitionEnd;
}

bool Opening::TransitionOut(){
	return kTransitionEnd;
}