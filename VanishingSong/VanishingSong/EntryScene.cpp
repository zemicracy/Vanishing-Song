#include "EntryScene.h"


EntryScene::EntryScene():
aetherClass::GameScene("Entry",GetManager())
{
}


EntryScene::~EntryScene()
{
}

//������
bool EntryScene::Initialize(){
	return true;
}

//�������
void EntryScene::Finalize(){

}

//1�t���[�����Ƃ̍X�V����
bool EntryScene::Updater(){
	return true;
}

//1�t���[�����Ƃ̕`�揈��
void EntryScene::Render(){

}

//UI�̕`�揈��
void EntryScene::UIRender(){

}

//�g�����W�V��������
bool EntryScene::TransitionIn(){
	return true;
}


bool EntryScene::TransitionOut(){
	return true;
}