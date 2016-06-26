#ifndef _AETHER_H
#define _AETHER_H
#pragma comment (lib,"Aether.lib")
#endif
#include <memory>
#include <AetherWindow.h>
#include <GameController.h>
#include <ConsoleWindow.h>
#include <GameSceneManager.h>
#include "Const.h"
#include "VanishingSongFrame.h"
#include "Load.h"
#include"SceneLogoView.h"
#include "SceneTitle.h"
using namespace aetherClass;

namespace{
	const int kError = -1;
	const int kEnd = 0;
	const int kWindowNum = 1;
}

// �G���g���[�|�C���g���쐬
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT){
	
	//�R���\�[���E�B���h�E�̍쐬
	ConsoleWindow::Create();

	//�X�}�[�g�|�C���^�錾
	std::unique_ptr<GameFrame>frame;

	//�X�}�[�g�|�C���^
	frame = std::make_unique<VanishingSongFrame>();
	
	//�E�B���h�E�N���X�̃I�u�W�F�N�g�쐬
	WindowBase* window = new AetherWindow();

	//�E�B���h�E�X�^�C���̐ݒ�
	WindowBase::WindowStyleDesc desc;
	desc._windowStyle = WS_OVERLAPPED | WS_SYSMENU;

	//�E�B���h�E�X�^�C�����E�B���h�E�I�u�W�F�N�g�ɐݒ�
	window->SetWindowStyles(desc);

	//�E�B���h�E���쐬���Ă���E�B���h�E�̃I�u�W�F�N�g�̃����o�֐�
	window->Create(kWindowName, Vector2(kWindowWidth, kWindowHeight));

	bool result = frame->Initialize(&window,kWindowNum,5000.0f,1.0f);

	//�G���[�`�F�b�N
	if (!result){
		return kError;
	}

	//��ԍŏ��̃V�[���ƃ��[�h��ʂ̐ݒ�
	GameSceneManager::SetCurrentScene(new SceneTitle());
	GameSceneManager::SetLoadScreen(new Load());
	frame->BackgroundColor(Color(0, 0, 0, 0));
	//���C�����[�v�̊J�n
	frame->GameRun();

	delete window;
	window = nullptr;

	//�t���[���I�u�W�F�N�g�̉��
	frame->Finalize();
	frame.release();
	//�R���\�[���E�B���h�E�̔j��
	ConsoleWindow::Close();
	return kEnd;
}
