#include "ModeSurvival.h"
#include "FragmentShader.h"

ModeSurvival::ModeSurvival()
{
}


ModeSurvival::~ModeSurvival()
{
}


bool ModeSurvival::mInitialize(GameManager::eSkillType skill,GameManager::eDay firstDay){
	// �e�N���X��Initialize�֐��̌Ăяo��
	Mode::mInitialize(skill, firstDay);
	return true;
}

void ModeSurvival::mUpdate(std::shared_ptr<ActionCommand> command, const float timeScale, const float nowTime){

	mGetPlayer()->mUpdate(timeScale, command);
	
	return;
}


void ModeSurvival::mRender(ShaderHash shaderHash){

	
	return;
}

void ModeSurvival::mUIRender(ShaderHash shaderHash){
	return;
}

// ���̓��ɐi�ނƂ��̏���������
void ModeSurvival::mNextDayInitialize(GameManager::eDay nextDay){

}

// �O�̓��̉������
void ModeSurvival::mPrevDayFinalize(GameManager::eDay prevDay){

}


