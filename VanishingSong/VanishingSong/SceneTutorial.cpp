#include "SceneTutorial.h"

const std::string SceneTutorial::Name = "Tutorial";
SceneTutorial::SceneTutorial():
GameScene(Name, GetManager())
{
}


SceneTutorial::~SceneTutorial()
{
}


//初期化
bool SceneTutorial::Initialize(){

	return true;
}

void SceneTutorial::Finalize(){

}

//1フレームごとの更新処理
bool SceneTutorial::Updater(){

	return true;
}

//1フレームごとの描画処理
void SceneTutorial::Render(){

}

//1フレームごとのui描画処理
void SceneTutorial::UIRender(){

}

//トランジション効果を設定
bool SceneTutorial::TransitionIn(){
	return kTransitionEnd;
}
bool SceneTutorial::TransitionOut(){
	return kTransitionEnd;
}