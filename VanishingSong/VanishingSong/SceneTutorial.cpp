#include "SceneTutorial.h"

const std::string SceneTutorial::Name = "Tutorial";
SceneTutorial::SceneTutorial():
GameScene(Name, GetManager())
{
}


SceneTutorial::~SceneTutorial()
{
}


//������
bool SceneTutorial::Initialize(){

	return true;
}

void SceneTutorial::Finalize(){

}

//1�t���[�����Ƃ̍X�V����
bool SceneTutorial::Updater(){

	return true;
}

//1�t���[�����Ƃ̕`�揈��
void SceneTutorial::Render(){

}

//1�t���[�����Ƃ�ui�`�揈��
void SceneTutorial::UIRender(){

}

//�g�����W�V�������ʂ�ݒ�
bool SceneTutorial::TransitionIn(){
	return kTransitionEnd;
}
bool SceneTutorial::TransitionOut(){
	return kTransitionEnd;
}