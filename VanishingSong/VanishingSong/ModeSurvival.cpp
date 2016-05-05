#include "ModeSurvival.h"
#include "FragmentShader.h"

ModeSurvival::ModeSurvival()
{
}


ModeSurvival::~ModeSurvival()
{
}


bool ModeSurvival::mInitialize(GameManager::eSkillType skill,GameManager::eDay firstDay){
	// 親クラスのInitialize関数の呼び出し
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

// 次の日に進むときの初期化処理
void ModeSurvival::mNextDayInitialize(GameManager::eDay nextDay){

}

// 前の日の解放処理
void ModeSurvival::mPrevDayFinalize(GameManager::eDay prevDay){

}


