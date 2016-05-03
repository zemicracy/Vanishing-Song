#include "ModePractice.h"


ModePractice::ModePractice()
{
}


ModePractice::~ModePractice()
{
}

bool ModePractice::mInitialize(GameManager::eSkillType skill,GameManager::eDay firstDay){
	// 親クラスのInitialize関数の呼び出し
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


// 次の日に進むときの初期化処理
void ModePractice::mNextDayInitialize(GameManager::eDay nextDay){

}

// 前の日の解放処理
void ModePractice::mPrevDayFinalize(GameManager::eDay prevDay){

}

