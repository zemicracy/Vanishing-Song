#include "ModePractice.h"


ModePractice::ModePractice()
{
}


ModePractice::~ModePractice()
{
}

bool ModePractice::mInitialize(GameManager::eSkillType skill,GameManager::eDay firstDay){
	// �e�N���X��Initialize�֐��̌Ăяo��
	Mode::mInitialize(skill, firstDay);
	return true;
}

void ModePractice::mUpdate(std::shared_ptr<ActionCommand> command, const float timeScale, const float nowTime){

	return;
}


void ModePractice::mRender(ShaderHash shaderHash){

}

void ModePractice::mUIRender(ShaderHash shaderHash){

}


// ���̓��ɐi�ނƂ��̏���������
void ModePractice::mNextDayInitialize(GameManager::eDay nextDay){

}

// �O�̓��̉������
void ModePractice::mPrevDayFinalize(GameManager::eDay prevDay){

}

