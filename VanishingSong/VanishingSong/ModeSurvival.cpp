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

void ModeSurvival::mUpdate(const float timeScale, const float nowTime){

	
	
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


