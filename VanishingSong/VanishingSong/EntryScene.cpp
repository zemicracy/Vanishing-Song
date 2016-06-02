#include "EntryScene.h"


EntryScene::EntryScene():
aetherClass::GameScene("Entry",GetManager())
{
}


EntryScene::~EntryScene()
{
}

//初期化
bool EntryScene::Initialize(){
	return true;
}

//解放処理
void EntryScene::Finalize(){

}

//1フレームごとの更新処理
bool EntryScene::Updater(){
	return true;
}

//1フレームごとの描画処理
void EntryScene::Render(){

}

//UIの描画処理
void EntryScene::UIRender(){

}

//トランジション効果
bool EntryScene::TransitionIn(){
	return true;
}


bool EntryScene::TransitionOut(){
	return true;
}