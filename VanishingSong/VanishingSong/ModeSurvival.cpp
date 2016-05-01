#include "ModeSurvival.h"


ModeSurvival::ModeSurvival()
{
}


ModeSurvival::~ModeSurvival()
{
}


bool ModeSurvival::mInitialize(GameManager::eDay firstDay){
	// �e�N���X��Initialize�֐��̌Ăяo��
	Mode::mInitialize(firstDay);
	return true;
}

void ModeSurvival::mUpdate(std::shared_ptr<ActionCommand> command, const float timeScale, const float nowTime){

	mGetPlayer()->mUpdate(timeScale, command);
	
	return;
}
void ModeSurvival::mRender(ShaderHash shaderHash){
	
	mGetPlayer()->mRender(shaderHash["texture"].get(), shaderHash["color"].get());
	mGetFieldArea()->mRender(shaderHash["color"].get());
	return;
}

// ���̓��ɐi�ނƂ��̏���������
void ModeSurvival::mNextDayInitialize(GameManager::eDay nextDay){

}

// �O�̓��̉������
void ModeSurvival::mPrevDayFinalize(GameManager::eDay prevDay){

}


