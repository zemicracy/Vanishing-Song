#include "SceneSurvival.h"
#include <iostream>
#include "Player.h"
SceneSurvival::SceneSurvival():
GameScene("Game", GetManager()) //SceneÇ≤Ç∆ÇÃñºëOÇê›íË
{
}


SceneSurvival::~SceneSurvival()
{
}

bool SceneSurvival::Initialize(){

	return true;
}

void SceneSurvival::Finalize(){

	return;
}

bool SceneSurvival::Updater(){

	return true;
}

void SceneSurvival::Render(){

	return;
}

void SceneSurvival::UIRender(){

	return;
}

bool SceneSurvival::TransitionIn(){

	return kTransitionEnd;
}

bool SceneSurvival::TransitionOut(){

	return kTransitionEnd;
}