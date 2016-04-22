#include "ScenePractice.h"
using namespace aetherClass;
const std::string ScenePractice::Name = "Practice";
ScenePractice::ScenePractice():
GameScene(Name, GetManager())
{
}


ScenePractice::~ScenePractice()
{
}

//初期化
bool ScenePractice::Initialize(){
	return true;
}


void ScenePractice::Finalize(){

}

//1フレームごとの更新処理
bool ScenePractice::Updater(){
	return true;
}

//1フレームごとの描画処理
void ScenePractice::Render(){

}

//1フレームごとのui描画処理
void ScenePractice::UIRender(){

}

//トランジション効果を設定
bool ScenePractice::TransitionIn(){
	return kTransitionEnd;
}

bool ScenePractice::TransitionOut(){
	return kTransitionEnd;
}

