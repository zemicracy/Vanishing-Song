#include "ScenePractice.h"
using namespace aetherClass;
const std::string ScenePractice::Name = "Practice";
ScenePractice::ScenePractice():
GameScene(Name, GetManager())
{
}


ScenePractice::~ScenePractice()
{
}

//������
bool ScenePractice::Initialize(){
	return true;
}


void ScenePractice::Finalize(){

}

//1�t���[�����Ƃ̍X�V����
bool ScenePractice::Updater(){
	return true;
}

//1�t���[�����Ƃ̕`�揈��
void ScenePractice::Render(){

}

//1�t���[�����Ƃ�ui�`�揈��
void ScenePractice::UIRender(){

}

//�g�����W�V�������ʂ�ݒ�
bool ScenePractice::TransitionIn(){
	return kTransitionEnd;
}

bool ScenePractice::TransitionOut(){
	return kTransitionEnd;
}

