#include "ModeSurvival.h"


ModeSurvival::ModeSurvival()
{
}


ModeSurvival::~ModeSurvival()
{
}


bool ModeSurvival::mInitialize(GameManager::eDay firstDay){
	// 親クラスのInitialize関数の呼び出し
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

// 次の日に進むときの初期化処理
void ModeSurvival::mNextDayInitialize(GameManager::eDay nextDay){

}

// 前の日の解放処理
void ModeSurvival::mPrevDayFinalize(GameManager::eDay prevDay){

}


