#include "SceneSurvival.h"
#include <iostream>

SceneSurvival::SceneSurvival():
GameScene("Game", GetManager()) //Scene‚²‚Æ‚Ì–¼‘O‚ğİ’è
{
}


SceneSurvival::~SceneSurvival()
{
}

bool SceneSurvival::Initialize(){
	std::cout << "Init" << std::endl;
	return true;
}

void SceneSurvival::Finalize(){
	std::cout << "Finalize" << std::endl;
	return;
}

bool SceneSurvival::Updater(){
	std::cout << "Updater" << std::endl;
	return true;
}

void SceneSurvival::Render(){
	std::cout << "Render" << std::endl;
	return;
}

void SceneSurvival::UIRender(){
	std::cout << "UI" << std::endl;
	return;
}

bool SceneSurvival::TransitionIn(){
	std::cout << "TranIn";
	return kTransitionEnd;
}

bool SceneSurvival::TransitionOut(){
	std::cout << "TranOut";
	return kTransitionEnd;
}